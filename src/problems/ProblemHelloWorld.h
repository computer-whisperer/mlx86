//
// Created by christian on 8/24/21.
//

#ifndef MLX86_HELLO_WORLD_H
#define MLX86_HELLO_WORLD_H

#include "problem.h"
#include <cstring>

class ProblemHelloWorld: public Problem {
    const char * goal = "I am mlx86!!!";
public:
    ProblemHelloWorld(): Problem(10){};
    explicit ProblemHelloWorld(const char* goal_in): Problem(strlen(goal_in)), goal(goal_in){};

    ProblemHelloWorld(const ProblemHelloWorld& problem) = default;

    ProblemHelloWorld * Clone() override
    {
        return new ProblemHelloWorld(*this);
    }

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
