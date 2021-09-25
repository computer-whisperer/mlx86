#ifndef CZYDIS_H
#define CZYDIS_H

void zydis_init();
void zydis_print_dissasembly(void * data, size_t data_len);
void zydis_print_instruction(void * data, size_t data_len, uint32_t address);

#endif
