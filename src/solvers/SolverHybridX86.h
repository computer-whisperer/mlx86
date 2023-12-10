//
// Created by christian on 9/13/21.
//

#ifndef MLX86_SOLVERHYBRIDX86_H
#define MLX86_SOLVERHYBRIDX86_H
#include "Solver.h"
#include <string>
#include "KVMExecutor.h"

class SolverHybridX86 : public Solver, public Problem
{
    KVMExecutor* executor;
public:
    static constexpr size_t max_code_len = 1000;
    uint8_t * code;
    explicit SolverHybridX86(size_t code_len_in);
    SolverHybridX86(const SolverHybridX86 &other);
    ~SolverHybridX86() override;

    SolverHybridX86 * Clone() override;

    void run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data = nullptr) override;

    double scalarTrial(U8 *data) override;
    void scrambler(U8* data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;
};

#endif //MLX86_SOLVERHYBRIDX86_H
