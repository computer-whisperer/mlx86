#ifndef PROBLEMS_PROBLEMS_H
#define PROBLEMS_PROBLEMS_H
#include <cstddef>
#include "types.h"

class Problem
{
public:
    explicit Problem(size_t data_len_in): data_len(data_len_in) {};
    virtual ~Problem() = default;
    virtual Problem* Clone() = 0;

    virtual double scalarTrial(U8* data);
	virtual void scrambler(U8* data);

    virtual void prettyPrintData(U8* data);
	virtual void dataInit(U8* data);
	const size_t data_len;

	void pureRandomScramble(U8 * data);
	static void textPrettyPrint(U8 * data);
	void hexPrettyPrint(U8 * data) const;
	void textDataInit(U8 * data) const;
};

struct Problem_T * new_problem_hybrid_x86_hyperparameters();

#endif
