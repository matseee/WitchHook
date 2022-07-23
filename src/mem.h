#pragma once
#include "framework.h"
#include <windows.h>
#include <vector>

#define INST_NOP				0x90
#define INST_CALL				0x08EB0000000215FF
#define INST_CALL_SIZE			16

typedef unsigned long long		u64;

namespace mem
{
	void Patch(BYTE* dst, BYTE* src, unsigned int size);
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);
	void Nop(BYTE* dst, unsigned int size);
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);

	uintptr_t FindDMAAddress(uintptr_t ptr, std::vector<unsigned int> offsets);

	bool Detour32(void* pSource, void* pDestination, int dwLen);
	bool Detour64(void* pSource, const void* pDestination, const u64 length = INST_CALL_SIZE);
}