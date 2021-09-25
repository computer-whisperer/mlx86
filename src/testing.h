#include <stddef.h>
#include "types.h"
#include "main.h"


#define TESTING_STARTING_HASH 4829176324

typedef U128 TESTING_HASH_T;

void testing_initialize();
void testing_deinitialize();
TESTING_HASH_T testing_add_data_to_hash(TESTING_HASH_T hash, void * data, size_t size);
#define TESTING_ADD_VALUE_TO_HASH(hash, value) hash = testing_add_data_to_hash((hash), &(value), sizeof(value))
#define TESTING_ADD_DATA_TO_HASH(hash, data, size) hash = testing_add_data_to_hash((hash), (data), (size))
U8 testing_assert_determinism(const char *func_name, TESTING_HASH_T input_hash, TESTING_HASH_T output_hash);
U8 testing_assert(const char * func_name, U8 value);
void testing_test();

