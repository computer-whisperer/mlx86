//
// Created by christian on 8/24/21.
//

#ifndef MLX86_HELLO_WORLD_H
#define MLX86_HELLO_WORLD_H

#include "problem.h"

class ProblemHelloWorld: public Problem {

public:
    ProblemHelloWorld(): Problem(10){};

    ProblemHelloWorld(const ProblemHelloWorld& problem) = default;

    double scalarTrial(U8 *data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;
};

class ProblemHelloWorldNonzero: public Problem {
public:
    ProblemHelloWorldNonzero(): Problem(10){};

    ProblemHelloWorldNonzero(const ProblemHelloWorldNonzero& problem) = default;

    double scalarTrial(U8 *data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;
};


#endif //MLX86_HELLO_WORLD_H
