//
// Created by christian on 8/24/21.
//

#ifndef MLX86_PROBLEMTRAVELLINGSALESMAN_H
#define MLX86_PROBLEMTRAVELLINGSALESMAN_H
#include <cstdint>
#include "problem.h"

class ProblemTravellingSalesman: public Problem
{
    static constexpr uint32_t max_node_count = 1000;
    using state_t = uint16_t;

public:
    ProblemTravellingSalesman(int node_count, int seed);

    ProblemTravellingSalesman(const ProblemTravellingSalesman &other) = default;

    ProblemTravellingSalesman * Clone() override
    {
        return new ProblemTravellingSalesman(*this);
    }

    double scalarTrial(U8 *data) override;
    void scrambler(U8* data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;

    state_t node_count;
    int seed;
    double x_coords[max_node_count];
    double y_coords[max_node_count];
};

#endif //MLX86_PROBLEMTRAVELLINGSALESMAN_H
