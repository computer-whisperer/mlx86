//
// Created by christian on 12/10/23.
//

/*

xor eax,eax
mov eax, [eax]
mov ecx, 10
div ecx
mov ebx, edx
mov ecx, 256
div ecx
add ebx, 4
mov [ebx], dl

*/

#include <cstring>
#include <SolverHybridX86.h>
#include <zydis.h>
uint8_t program[] = {
0x31, 0xC0, 0x8B, 0x00, 0xB9, 0x1b, 0x00, 0x00, 0x00, 0xF7, 0xF1, 0x89, 0xD3, 0xB9, 0x00, 0x01, 0x00, 0x00, 0xF7, 0xF1, 0x83, 0xC3, 0x04, 0x88, 0x13
};

int main(int argc, const char *const *argv)
{
  zydis_init();
  uint8_t data[256];
  memset(data, 0x90, sizeof(data));
  memcpy(data, program, sizeof(program));
  auto solver_hybrid_x86 = new SolverHybridX86(sizeof(data));
  float value = solver_hybrid_x86->scalarTrial(data);
  printf("score: %f\n", value);
  solver_hybrid_x86->prettyPrintData(data);
}