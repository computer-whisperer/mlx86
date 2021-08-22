#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Zydis/Zydis.h>
#include "types.h"
#include "zydis.h"

static ZydisDecoder decoder;
static ZydisFormatter formatter;

void zydis_init()
{

    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LEGACY_32, ZYDIS_ADDRESS_WIDTH_32);

    // Initialize formatter. Only required when you actually plan to do instruction
    // formatting ("disassembling"), like we do here
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

}

void zydis_print_dissasembly(void * data, size_t data_len)
{

	// Loop over the instructions in our buffer.
	// The runtime-address (instruction pointer) is chosen arbitrary here in order to better
	// visualize relative addressing
	ZyanU64 runtime_address = 0x1000000;
	size_t offset = 0;
	ZydisDecodedInstruction instruction;
	while (offset < data_len)
	{
		U8 rc = ZydisDecoderDecodeBuffer(&decoder, data + offset, data_len - offset,
				&instruction);
		if (rc)
		{
			break;
		}

		// Skip over nops
		if (instruction.opcode != 0x90)
		{
			// Print current instruction pointer.
			printf("0x%08X ", runtime_address);

			// Format & print the binary instruction structure to human readable format
			char buffer[256];
			ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
				runtime_address);
			puts(buffer);
		}

		offset += instruction.length;
		runtime_address += instruction.length;
	}
}

void zydis_print_instruction(void * data, size_t data_len, U32 address)
{
	ZydisDecodedInstruction instruction;
	U8 rc = ZydisDecoderDecodeBuffer(&decoder, data, data_len,
			&instruction);
	if (!rc)
	{
		// Print current instruction pointer.
		printf("0x%08X ", address);

		// Format & print the binary instruction structure to human readable format
		char buffer[256];
		ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
				address);
		puts(buffer);
	}
}
