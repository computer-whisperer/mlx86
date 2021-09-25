#include "testing.h"
#include "christian_utils.h"
#include <cstdlib>
#include <cstdio>
#include <strings.h>

#ifndef DO_TESTING

void testing_initialize(){};
void testing_deinitialize(){};
TESTING_HASH_T testing_add_data_to_hash(TESTING_HASH_T hash, void * data, size_t size){return 0;};
U8 testing_assert_determinism(const char *func_name, TESTING_HASH_T input_hash, TESTING_HASH_T output_hash){return 1;};
U8 testing_assert(const char * func_name, U8 value){return value;};
void testing_test(){};

#else

struct DETERMINISM_TABLE_ENTRY_T
{
	U8 populated;
	TESTING_HASH_T input_hash;
	TESTING_HASH_T output_hash;
};

U64 determinism_table_len;
U64 determinism_table_num_populated;
static struct DETERMINISM_TABLE_ENTRY_T * determinism_table;

void testing_initialize()
{
	determinism_table_num_populated = 0;
	determinism_table_len = 256;
	determinism_table = malloc(sizeof(struct DETERMINISM_TABLE_ENTRY_T) * determinism_table_len);
	for (U64 i = 0; i < determinism_table_len; i++)
	{
		determinism_table[i].populated = 0;
	}
}

void testing_deinitialize()
{
	free(determinism_table);
}

TESTING_HASH_T testing_add_data_to_hash(TESTING_HASH_T hash, void * data, size_t size)
{
	TESTING_HASH_T buff;
	qhashmurmur3_128(data, size, &buff);
    return buff + hash<<2;
}

U8 testing_assert_determinism(U8 * test_name, TESTING_HASH_T input_hash, TESTING_HASH_T output_hash)
{
	U8 ret = 2;

	// Append test_name to hash
	input_hash = testing_add_data_to_hash(input_hash, test_name, sizeof(test_name));
	U64 start_idx = (input_hash>>19)%determinism_table_len;
	// Quadratic probing

	for (U64 o = 0; o < determinism_table_len; o++)
	{
		U64 i = (start_idx + o*o)%determinism_table_len;
		if (!determinism_table[i].populated)
		{
			determinism_table_num_populated++;
			determinism_table[i].populated = 1;
			determinism_table[i].input_hash = input_hash;
			determinism_table[i].output_hash = output_hash;
			ret = testing_assert(test_name, 1);
			break;
		}
		else
		{
			if (determinism_table[i].input_hash == input_hash)
			{
				ret = testing_assert(test_name, (determinism_table[i].output_hash == output_hash));
				break;
			}
			else
			{
				// Fall through, try again.
			}
		}
	}

#ifdef DO_TESTING_TESTING
	if (!testing_assert("testing_assert_determinism_1", ret < 2))
	{
		printf("Hi!");
	}
#endif

	if (determinism_table_num_populated > (determinism_table_len/2))
	{
		// Expand table
		U64 last_table_len = determinism_table_len;
		struct DETERMINISM_TABLE_ENTRY_T * last_determinism_table = determinism_table;

		// New table
		determinism_table_len *= 2;
		determinism_table = malloc(sizeof(struct DETERMINISM_TABLE_ENTRY_T) * determinism_table_len);
		for (U64 i = 0; i < determinism_table_len; i++)
		{
			determinism_table[i].populated = 0;
		}

		// Copy entries
		for (U64 j = 0; j < last_table_len; j++)
		{
			if (last_determinism_table[j].populated)
			{
				TESTING_HASH_T input_hash = last_determinism_table[j].input_hash;
				TESTING_HASH_T output_hash = last_determinism_table[j].output_hash;
				U64 start_idx = (input_hash>>19)%determinism_table_len;
				// Quadratic probing
				for (U64 o = 0; o < determinism_table_len; o++)
				{
					U64 i = (start_idx + o*o)%determinism_table_len;
					if (!determinism_table[i].populated)
					{
						determinism_table[i].populated = 1;
						determinism_table[i].input_hash = input_hash;
						determinism_table[i].output_hash = output_hash;
						break;
					}
					else
					{
						// Fall through, try again.
#ifdef DO_TESTING_TESTING
						testing_assert("testing_assert_determinism_2", determinism_table[i].input_hash != input_hash);
#endif
					}
				}
			}
		}

		// Clear last table
		free(last_determinism_table);

	}
	return ret;
}

static U8 mute_self = 0;

U8 testing_assert(U8 * test_name, U8 value)
{
	if (!value && !mute_self)
	{
		printf("Assertion %s failed!\n", test_name);
	}
	return value;
}

static U32 testing_test_deterministic(U32 a, U8 b)
{
	return a * b + b*b*a;
}

static U32 testing_test_non_deterministic(U32 a, U8 b)
{
	static U8 p = 0;
	p++;
	return a * b + b*b*a + p;
}

void testing_test()
{
	mute_self = 1;

	for (U32 a = 0; a < 90; a++)
	{
		for (U32 b = 0; b < 90; b++)
		{
			TESTING_HASH_T input_hash = TESTING_STARTING_HASH;
			TESTING_ADD_VALUE_TO_HASH(input_hash, a);
			TESTING_ADD_VALUE_TO_HASH(input_hash, b);


			for (U32 i = 0; i < 20; i++)
			{
				TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
				U32 output_val = testing_test_deterministic(a, b);
				TESTING_ADD_VALUE_TO_HASH(output_hash, output_val);
				if (!testing_assert_determinism("d", input_hash, output_hash))
				{
					printf("Deterministic Fail!\n");
				}
			}

			TESTING_HASH_T output_hash = TESTING_STARTING_HASH;
			U32 output_val = testing_test_non_deterministic(a, b);
			TESTING_ADD_VALUE_TO_HASH(output_hash, output_val);
			if (!testing_assert_determinism("nd", input_hash, output_hash))
			{
				printf("Non Deterministic Start Fail!\n");
			}

			for (U32 i = 0; i < 20; i++)
			{
				output_hash = TESTING_STARTING_HASH;
				output_val = testing_test_non_deterministic(a, b);
				TESTING_ADD_VALUE_TO_HASH(output_hash, output_val);
				if (testing_assert_determinism("nd", input_hash, output_hash))
				{
					printf("Non Deterministic Fail!\n");
				}
			}

		}
	}
}

#endif
