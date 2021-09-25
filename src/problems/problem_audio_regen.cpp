//
// Created by christian on 9/10/21.
//

#include <christian_utils.h>
#include "problem_audio_regen.h"
#include <kfr/all.hpp>
#include <complex>

using namespace kfr;
using namespace std;


thread_local std::vector<kfr::univector<kfr::complex<double>, 128>> cached_test_ft_output;
thread_local std::vector<kfr::univector<kfr::complex<double>, 128>> cached_test_ft_input;

problem_audio_regen::problem_audio_regen(std::string input_wav_fname_in) :
Problem(max_sample_count),
input_wav_fname(std::move(input_wav_fname_in)),
dft(dft_len)
{
    inputAudioFile.load(input_wav_fname);
    sample_num = inputAudioFile.getNumSamplesPerChannel();

    num_fts = ((sample_num-dft_len)/dft_period);

    univector<uint8_t> temp(dft.temp_size);

    for (int i = 0; i < num_fts; i++)
    {
        int offset = i*dft_period;
        univector<kfr::complex<double>, dft_len> input_data, output_data;
        for (int j = 0; j < dft_len; j++)
        {
            input_data[j] = inputAudioFile.samples[0][offset + j];
        }

        dft.execute(output_data, input_data, temp);

        input_ft_data.push_back(output_data);
    }
}

void problem_audio_regen::dataInit(U8 *data) {
    memset(data, 127, data_len);
}

void problem_audio_regen::prettyPrintData(U8 *data) {
    print_data_as_hex(data, 1000);
}

double problem_audio_regen::scalarTrial(U8 *data) {
    cached_test_ft_input.resize(num_fts);
    cached_test_ft_output.resize(num_fts);

    int num_tests = 0;
    double total_error = 0;
    univector<uint8_t> temp(dft.temp_size);
    for (int i = 0; i < num_fts; i++)
    {
        int offset = i*dft_period;
        univector<kfr::complex<double>, dft_len> input_data;
        for (int j = 0; j < dft_len; j++)
        {
            input_data[j] = ((double)data[offset+j]-0x80)/(double)0x80;
        }

        // Compare with cached result
        bool recalculate = false;
        for (int j = 0; j < dft_len; j++)
        {
            if (input_data[j].real() != cached_test_ft_input[i][j].real())
            {
                recalculate = true;
                break;
            }
        }
        if (recalculate)
        {
            cached_test_ft_input[i] = input_data;
            dft.execute(cached_test_ft_output[i], input_data, temp);
        }

        // Score it
        for (int j = 0; j < dft_len; j++)
        {
            total_error += abs(input_ft_data[i][j].real() - cached_test_ft_output[i][j].real());
            num_tests++;
        }
    }

    return 1.0 - (total_error/num_tests);
}

void problem_audio_regen::scrambler(U8 *data) {
    unsigned char * to_change = data + fast_rand()%(sample_num);
    const int max_bump = 32;
    *to_change = ((int)*to_change) + fast_rand()%(max_bump*2) - max_bump;
}

void problem_audio_regen::export_wav(std::string output_wave_fname, U8* data) {
    AudioFile<double> audioFile;
    audioFile.setNumChannels(1);
    audioFile.setNumSamplesPerChannel(sample_num);
    audioFile.setBitDepth(8);
    audioFile.setSampleRate(inputAudioFile.getSampleRate());
    for (int i = 0; i < sample_num; i++)
    {
        audioFile.samples[0][i] = ((double)data[i] - 0x80)/0x80;
    }
    audioFile.save(output_wave_fname);
}

