//
// Created by christian on 9/10/21.
//

#ifndef MLX86_PROBLEM_AUDIO_REGEN_H
#define MLX86_PROBLEM_AUDIO_REGEN_H
#include <string>
#include <utility>
#include "problem.h"
#include "AudioFile.h"
#include <kfr/base.hpp>
#include <kfr/dft.hpp>
#include <complex>

class problem_audio_regen : public Problem {

    static constexpr int max_sample_count = 100000;
    std::string input_wav_fname;
    AudioFile<double> inputAudioFile;

    static constexpr int dft_len = 128;
    static constexpr int dft_period = dft_len/2;

    kfr::dft_plan<kfr::fbase> dft;

    int sample_num;
    int num_fts;

    std::vector<kfr::univector<kfr::complex<double>, dft_len>> input_ft_data;

public:
    problem_audio_regen(std::string input_wav_fname_in);

    double scalarTrial(U8 *data) override;
    void scrambler(U8* data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;

    void export_wav(std::string output_wave_fname, U8* data);
};


#endif //MLX86_PROBLEM_AUDIO_REGEN_H
