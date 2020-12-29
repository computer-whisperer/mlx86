#include <string.h>
#include "problems/problems.h"
#include "utils.h"
#include "types.h"

void pure_random_scramble(struct Problem_T * problem, U8 * data)
{
	unsigned char * to_change = data + fast_rand()%(problem->data_len);
	*to_change = (U8)fast_rand();
}

void text_pretty_print(struct Problem_T * problem, U8 * data)
{
	printf(data);
	putchar('\n');
}

void text_data_init(struct Problem_T * problem, U8 * data)
{
	memset(data, ' ', problem->data_len);
}
