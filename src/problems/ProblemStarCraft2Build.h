//
// Created by christian on 11/1/21.
//

#ifndef MLX86_PROBLEM_STARCRAFT2_BUILD_H
#define MLX86_PROBLEM_STARCRAFT2_BUILD_H

class ProblemStarCraft2Build : public Problem
{
public:
    ProblemStarCraft2Build(): Problem(10){};

    ProblemStarCraft2Build(const ProblemStarCraft2Build& problem) = default;

    double scalarTrial(U8 *data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;

    void scrambler(U8* data) override;
};

#endif //MLX86_PROBLEM_STARCRAFT2_BUILD_H
