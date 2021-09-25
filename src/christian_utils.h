#ifndef CHRISTIAN_UTILS_H
#define CHRISTIAN_UTILS_H

#include <cstddef>
#include "types.h"

#define FAST_RAND_MAX 32767

double getUnixTime(void);
void seed_fast_rand(int seed);
int fast_rand(void);
unsigned int get_fast_rand_seed();
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

U32 qhashmurmur3_32(const void *data, size_t nbytes);
U8 qhashmurmur3_128(const void *data, size_t nbytes, void *retbuf);

#endif /* UTILS_H */
