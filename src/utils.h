#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include "types.h"

#define FAST_RAND_MAX 32767

double getUnixTime(void);
void seed_fast_rand(int seed);
int fast_rand(void);
int fast_rand_seeded(int m_seed);
double double_norm_rand(double mean, double std_dev);
double double_uniform_rand(int seed);
unsigned long hash(unsigned char *str);
unsigned long hash_with_len(unsigned char *str, size_t len);
double fastPow(double a, double b);
int ceil_log2(unsigned long long x);
void print_data_as_hex(U8 * data, U32 data_len);
void print_data_as_hex_10wrap(U8 * data, U32 data_len);

void set_process_offset(U8 offset);
U8 get_process_offset();
#endif /* UTILS_H */
