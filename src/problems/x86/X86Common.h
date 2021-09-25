#ifndef X86COMMON_H
#define X86COMMON_H

#include "problems/problem.h"

void x86_data_init(uint8_t * data, size_t data_len);
void x86_pretty_print(uint8_t * data, size_t data_len);
void x86_basic_scramble(uint8_t * data, size_t data_len);

#endif