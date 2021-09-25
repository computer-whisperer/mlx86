//
// Created by christian on 9/24/21.
//

#ifndef MLX86_PROBLEMX86STRINGMATCH_H
#define MLX86_PROBLEMX86STRINGMATCH_H
#include <string>
#include <utility>
#include <KVMExecutor.h>
#include "problem.h"

class ProblemX86StringMatch : public Problem
{
    std::string goal;
    KVMExecutor * executor;
public:
    ProblemX86StringMatch(std::string goal_in, size_t code_len);

    ProblemX86StringMatch(const ProblemX86StringMatch &other);

    ProblemX86StringMatch * Clone() override
    {
        return new ProblemX86StringMatch(*this);
    }

    double scalarTrial(U8 *data) override;
    void scrambler(U8* data) override;

    void prettyPrintData(U8 *data) override;

    void dataInit(U8 *data) override;

    ~ProblemX86StringMatch() override;
};


#endif //MLX86_PROBLEMX86STRINGMATCH_H
