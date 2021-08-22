#include <string.h>
#include <stdio.h>
#include "problems/problems.h"
#include "types.h"
#include "utils.h"
#include "zydis.h"
#define INS_LEN 10

// All instructions obey modrm
struct known_instruction
{
	U16 opcode;
	U8 has_modrm;
	U8 reg_val;
	U8 imm_len;
};

// 0xFF represents no such opcode
// Any other value is index of opcode in known_instructions table
U8 known_instructions_by_opcode[256];

struct known_instruction known_instructions[] = {
	// ADD
	{0x00, 1, 0xFF, 0},
	{0x01, 1, 0xFF, 0},
	{0x02, 1, 0xFF, 0},
	{0x03, 1, 0xFF, 0},
	{0x04, 0, 0xFF, 1},
	{0x05, 0, 0xFF, 4},
	// PUSH
	{0x06, 0, 0xFF, 0},
	{0x0E, 0, 0xFF, 0},
	// POP
	{0x07, 0, 0xFF, 0},
	// OR
	{0x08, 1, 0xFF, 0},
	{0x09, 1, 0xFF, 0},
	{0x0A, 1, 0xFF, 0},
	{0x0B, 1, 0xFF, 0},
	{0x0C, 0, 0xFF, 1},
	{0x0D, 0, 0xFF, 4},
	// AND
	{0x20, 1, 0xFF, 0},
	{0x21, 1, 0xFF, 0},
	{0x22, 1, 0xFF, 0},
	{0x23, 1, 0xFF, 0},
	{0x24, 0, 0xFF, 1},
	{0x25, 0, 0xFF, 4},
	// SUB
	{0x28, 1, 0xFF, 0},
	{0x29, 1, 0xFF, 0},
	{0x2A, 1, 0xFF, 0},
	{0x2B, 1, 0xFF, 0},
	{0x2C, 0, 0xFF, 1},
	{0x2D, 0, 0xFF, 4},
	// XOR
	{0x30, 1, 0xFF, 0},
	{0x31, 1, 0xFF, 0},
	{0x32, 1, 0xFF, 0},
	{0x33, 1, 0xFF, 0},
	{0x34, 0, 0xFF, 1},
	{0x35, 0, 0xFF, 4},
	// CMP
	{0x38, 1, 0xFF, 0},
	{0x39, 1, 0xFF, 0},
	{0x3A, 1, 0xFF, 0},
	{0x3B, 1, 0xFF, 0},
	{0x3C, 0, 0xFF, 1},
	{0x3D, 0, 0xFF, 4},
	// IMUL
	{0x69, 1, 0xFF, 4},
	{0x6B, 1, 0xFF, 1},
	// Jumps
	{0x72, 0, 0xFF, 1},//JB
	{0x73, 0, 0xFF, 1},//JNB
	{0x74, 0, 0xFF, 1},//JZ
	{0x75, 0, 0xFF, 1},//JNZ
	{0x76, 0, 0xFF, 1},//JBE
	{0x77, 0, 0xFF, 1},//JNBE
	// TEST
	{0x84, 1, 0xFF, 0},
	{0x85, 1, 0xFF, 0},
	{0xA8, 0, 0xFF, 1},
	{0xA9, 0, 0xFF, 4},
	// MOV
	{0x88, 1, 0xFF, 0},
	{0x89, 1, 0xFF, 0},
	{0x8A, 1, 0xFF, 0},
	{0x8B, 1, 0xFF, 0},
	{0xA0, 0, 0xFF, 4},
	{0xA1, 0, 0xFF, 4},
	{0xA2, 0, 0xFF, 4},
	{0xA3, 0, 0xFF, 4},
	{0xC6, 1, 0x00, 1},
	{0xC7, 1, 0x00, 4},
	// Rotate/shift
	{0xC0, 1, 0x00, 1},//ROL
	{0xC0, 1, 0x01, 1},//ROR
	{0xC0, 1, 0x02, 1},//RCL
	{0xC0, 1, 0x03, 1},//RCR
	{0xC0, 1, 0x04, 1},//SHL
	{0xC0, 1, 0x05, 1},//SHR
	{0xC0, 1, 0x06, 1},//SAL
	{0xC0, 1, 0x07, 1},//SAR
	// NOP
	{0x90, 0, 0xFF, 0},
	// HLT
	{0xF4, 0, 0xFF, 0}
};

static U8 is_lookup_table_ready = 0;

U8 get_disp_len(U8 modrm)
{
	if ((modrm&0xC0) == 0x00)
	{
		if ((modrm&0x07) == 0x05)
		{
			return 4;
		}
		else
		{
			return 0;
		}
	}

	else if ((modrm&0xC0) == 0x40)
	{
		return 1;
	}

	else if ((modrm&0xC0) == 0x80)
	{
		return 4;
	}

	else
	{
		return 0;
	}
}

void x86_data_init(struct Problem_T * problem, U8 *data)
{
	memset(data, 0x90, problem->data_len);
}

void x86_basic_scramble(struct Problem_T * problem, U8 * data)
{
	// Build lookup table
	if (!is_lookup_table_ready)
	{
		memset(known_instructions_by_opcode, 0xFF, sizeof(known_instructions_by_opcode));
		for (U8 i = 0; i < (sizeof(known_instructions)/sizeof(known_instructions[0])); i++)
		{
			known_instructions_by_opcode[known_instructions[i].opcode] = i;
		}
		is_lookup_table_ready = 1;
	}

	// Assume data is decent x86
	U32 inst_count = problem->data_len/INS_LEN;
	// Pick instruction at random
	U32 inst_idx = fast_rand()%inst_count;
	// Disassemble existing instructions
	U8 opcode = data[inst_idx*INS_LEN + 0];
	U8 instruction_idx = known_instructions_by_opcode[opcode];
	U8 modrm = 0;
	U8 i = 1;
	U8 imm[4] = {0, 0, 0, 0};
	U8 disp_len = 0;
	U8 disp[4] = {0, 0, 0, 0};
	if (known_instructions[instruction_idx].has_modrm)
	{
		modrm = data[inst_idx*INS_LEN + i];
		i++;
		disp_len = get_disp_len(modrm);
	}

	for (U8 j = 0; j < disp_len; j++)
	{
		disp[j] = data[inst_idx*INS_LEN + i];
		i++;
	}

	for (U8 j = 0; j < known_instructions[instruction_idx].imm_len; j++)
	{
		imm[j] = data[inst_idx*INS_LEN + i];
		i++;
	}

	// Should we replace this instruction?
	if ((instruction_idx == 0xFF) || (fast_rand()%8 == 0))
	{
		opcode = known_instructions[fast_rand()%(sizeof(known_instructions)/sizeof(known_instructions[0]))].opcode;
		instruction_idx = known_instructions_by_opcode[opcode];

		if (!known_instructions[instruction_idx].has_modrm)
		{
			disp_len = 0;
		}
		else
		{
			// Check if we must set reg to something
			if (known_instructions[instruction_idx].reg_val != 0xFF)
			{
				modrm = (modrm&0xC7)|((known_instructions[instruction_idx].reg_val<<3)&0x38);
			}
			disp_len = get_disp_len(modrm);
		}
	}

	if (known_instructions[instruction_idx].has_modrm)
	{
		// Should we change mod?
		if ((fast_rand()%8) == 0)
		{
			// All values equally likely
			modrm = (modrm&0x3F)|(fast_rand()&0xC0);
		}
		// Should we change reg?
		if ((known_instructions[instruction_idx].reg_val == 0xFF) && ((fast_rand()%8) == 0))
		{
			// All values equally likely
			modrm = (modrm&0xC7)|(fast_rand()&0x28);
		}
		// Should we change r/m?
		if ((fast_rand()%8) == 0)
		{
			// All values equally likely
			modrm = (modrm&0xF8)|(fast_rand()&0x07);

			// Except avoid 0x04 for now (invokes SIB byte)
			if ((modrm&0x07) == 0x04)
			{
				modrm = (modrm&0xF8);
			}
		}
		disp_len = get_disp_len(modrm);
	}

	if ((disp_len > 0) && ((fast_rand()%8) == 0))
	{
		// Randomise disp
		S32 val = (fast_rand() + fast_rand() + fast_rand() + fast_rand() - ((32767*4)/2))/100;
		if (val < 0)
			val = -val;
		*((U32*)disp) = (U32)val;
	}

	if ((known_instructions[instruction_idx].imm_len > 0) && ((fast_rand()%8) == 0))
	{
		// Randomise imm
		S32 val = (fast_rand() + fast_rand() + fast_rand() + fast_rand() - ((32767*4)/2))/100;
		if (val < 0)
			val = -val;
		*((U32*)imm) = (U32)val;
	}

	// Rebuild new instruction
	data[inst_idx*INS_LEN] = opcode;
	i = 1;
	if (known_instructions[instruction_idx].has_modrm)
	{
		data[inst_idx*INS_LEN + i] = modrm;
		i++;
	}

	for (U8 j = 0; j < disp_len; j++)
	{
		data[inst_idx*INS_LEN + i] = disp[j];
		i++;
	}

	for (U8 j = 0; j < known_instructions[instruction_idx].imm_len; j++)
	{
		data[inst_idx*INS_LEN + i] = imm[j];
		i++;
	}

	// Pad with NOPs
	while (i < INS_LEN)
	{
		data[inst_idx*INS_LEN + i] = 0x90;
		i++;
	}
}

void x86_pretty_print(struct Problem_T * problem, U8 * data)
{
	size_t len = problem->data_len;
	if (len > 0x100)
		len = 0x100;
	zydis_print_dissasembly(data, len);
}
