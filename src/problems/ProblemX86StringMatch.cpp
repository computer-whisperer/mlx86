//
// Created by christian on 9/24/21.
//

#include <X86Common.h>
#include <cstring>
#include <utility>
#include "ProblemX86StringMatch.h"
#include "KVMExecutor.h"

ProblemX86StringMatch::ProblemX86StringMatch(std::string goal_in, size_t code_len) : goal(std::move(goal_in)), Problem(code_len), executor(nullptr)
{}

double ProblemX86StringMatch::scalarTrial(U8 *data)
{
    if (executor == nullptr)
    {
        executor = new KVMExecutor(goal.length(), data_len);
    }
    executor->sanitize();
    U8 * program_mem = executor->program_memory;
    U8 * io_mem = executor->io_memory;

    memset(io_mem, 0, goal.length());
    memcpy(program_mem, data, data_len);
    int did_hang = executor->run();

    double score = 0;

    U32 l = goal.length();

    for (int i = 0; i < l; i++)
    {
        int error = goal[i] - io_mem[i];
        if (error < 0)
            error = -error;
        if (error > 20)
            error = 20;
        if (error == 0) {
            score += 2;
        }
        score += 1.0f-(static_cast<float>(error)/20.0f);
    }

    if (did_hang) {
        return -100;
    }

    return (score/3)/((double)l);
}

void ProblemX86StringMatch::scrambler(U8 *data) {
    //x86_basic_scramble(data, data_len);
    pureRandomScramble(data);
}

void ProblemX86StringMatch::prettyPrintData(U8 *data) {
    U8 * program_mem = executor->program_memory;
    U8 * io_mem = executor->io_memory;

    memset(io_mem, 0, goal.length());
    memcpy(program_mem, data, data_len);
    int did_hang = executor->run();
    for (uint32_t i = 0; i < goal.length(); i++) {
        printf("%02x ", io_mem[i]);
    }
    printf("\n");
    io_mem[goal.length()] = 0;
    printf("%s\n", (char*)io_mem);
    x86_pretty_print(data, data_len);
}

void ProblemX86StringMatch::dataInit(U8 *data) {
    x86_data_init(data, data_len);
}

ProblemX86StringMatch::ProblemX86StringMatch(const ProblemX86StringMatch &other)
: Problem(other.data_len), goal(other.goal), executor(nullptr) {}

ProblemX86StringMatch::~ProblemX86StringMatch() {
    delete executor;
}
