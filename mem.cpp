#include "pch.h"
#include "framework.h"
#include "mem.h"

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	memcpy(dst, src, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	WriteProcessMemory(hProcess, dst, src, size, nullptr);
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, 0x90, size);

	PatchEx(dst, nopArray, size, hProcess);
	delete[] nopArray;
}

uintptr_t mem::FindDMAAddress(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}
	return addr;
}

bool mem::Detour32(void* pSource, void* pDestination, int dwLen) {
	if (dwLen < 5) {
		return false;
	}

	DWORD curProtection;
	VirtualProtect(pSource, dwLen, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(pSource, 0x90, dwLen);

	DWORD relativeAddress = ((DWORD)pDestination - (DWORD)pSource) - 5;

	*(BYTE*)pSource = 0xE9;
	*(DWORD*)((DWORD)pSource + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(pSource, dwLen, curProtection, &temp);

	return true;
}

bool mem::Detour64(void* pSource, const void* pDestination, const u64 length)
{
	if (pSource && pDestination)
	{
		if (length < INST_CALL_SIZE)
			return false;

		DWORD curProtection;
		VirtualProtect(pSource, length, PAGE_EXECUTE_READWRITE, &curProtection);

		if (length > INST_CALL_SIZE)
			memset(pSource, INST_NOP, length);

		*(u64*)pSource = INST_CALL;
		*(u64*)((u64)pSource + 8) = (u64)pDestination;

		DWORD temp;
		VirtualProtect(pSource, length, curProtection, &temp);

		return true;
	}
	else
	{
		return false;
	}
}

