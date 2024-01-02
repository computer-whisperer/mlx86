//
// Created by christian on 9/13/21.
//

#ifndef MLX86_SOLVERHYBRIDX86_H
#define MLX86_SOLVERHYBRIDX86_H
#include "Solver.h"
#include <string>
#include <vector>

#include "KVMExecutor.h"

class SolverHybridX86 : public Solver, public Problem
{
    KVMExecutor* executor;
public:
    static constexpr size_t max_code_len = 512;
    uint8_t * code;
    explicit SolverHybridX86(size_t code_len_in);
    explicit SolverHybridX86(std::string fname);
    SolverHybridX86(const SolverHybridX86 &other);
    ~SolverHybridX86() override;

    std::vector<Problem*> problems;

    SolverHybridX86 * Clone() override;

    void run(Problem *problem, struct REPORTER_MEM_T * reporter_mem, double score_limit, U32 trial_limit, struct SolverResults_T * results_out, uint8_t* starting_data = nullptr) override;

    double scalarTrial(U8 *data) override;
    void scrambler(U8* data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;

    uint32_t inner_rng_seed = 123;
    uint32_t inner_rng();
};

#endif //MLX86_SOLVERHYBRIDX86_H
