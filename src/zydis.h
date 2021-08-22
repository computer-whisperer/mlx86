#ifndef ZYDIS_H
#define ZYDIS_H

void zydis_init();
void zydis_print_dissasembly(void * data, size_t data_len);
void zydis_print_instruction(void * data, size_t data_len, U32 address);

#endif
