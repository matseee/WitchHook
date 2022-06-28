#include "pch.h"
#include "framework.h"
#include "mem.h"

struct MapPointerCoordinates {
    float x;
    float y;
};

struct PlayerCoordinates {
    double x;
    double y;
    double z;
};

FILE* pFile;

extern "C" void VitalityDetour();

void StartMenu()
{
    AllocConsole();
    freopen_s(&pFile, "CONOUT$", "w", stdout);

    std::cout << "WitchHook injected successfully :))" << std::endl << std::endl;
    std::cout << " F1: Add 500 gold" << std::endl;
    std::cout << " F2: Unl. vitality and stamina" << std::endl;
    std::cout << " F3: Teleport to markpoint" << std::endl;
    std::cout << "F10: Detach WitchHook" << std::endl << std::endl;
}

void StopMenu()
{
    if (pFile)
    {
        fclose(pFile);
    }

    FreeConsole();
}

DWORD __stdcall Thread(HMODULE hModule)
{
    DWORD vitalityAddress = 0xE3BF93;
    bool bVitiality = false;

    StartMenu();

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"witcher3.exe");

    while (true) 
    { 
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            uintptr_t gold = mem::FindDMAAddress(moduleBase + 0x02BC7AD0, { 0x1A8, 0x40, 0x1B0, 0x10, 0x140, 0x684 });

            *(int*)gold = (* (int*)gold) + 500;

            std::cout << "Added 500 gold" << std::endl;
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_F2) & 1)
        {
            bVitiality = !bVitiality;

            if (bVitiality)
            {
                mem::Detour64((void*)(moduleBase + vitalityAddress), VitalityDetour);
            }
            else
            {
                mem::Patch((BYTE*)(moduleBase + vitalityAddress), (BYTE*)"\xF3\x0F\x11\x34\x88\x83\xFB\x06\x75\x17\x8B\x05\xF1\x20\xF2\x01", 16);
            }
        }

        if (GetAsyncKeyState(VK_F3) & 1)
        {
            PlayerCoordinates* playerCoordinates = (PlayerCoordinates*)mem::FindDMAAddress(moduleBase + 0x02AA4148, { 0x28, 0x10, 0x1C0, 0x38, 0x130, 0x1518, 0x10, 0x78, 0x1C0 });
            MapPointerCoordinates* mapPointerCoordinates = (MapPointerCoordinates*)mem::FindDMAAddress(moduleBase + 0x02AA7820, { 0x48, 0x114, 0x8 });

            (*playerCoordinates).x = (*mapPointerCoordinates).x;
            (*playerCoordinates).y = (*mapPointerCoordinates).y;
            (*playerCoordinates).z = 250.0;
        }
        
        if (GetAsyncKeyState(VK_F10) & 1)
        {
            break;
        }

        Sleep(20);
    }

    StopMenu();
    FreeLibraryAndExitThread((HINSTANCE)hModule, 1);
    return 0;
}

BOOL APIENTRY DllMain(
    HMODULE hModule, 
    DWORD  ulReasonForCall, 
    LPVOID lpReserved
)
{   
    if (ulReasonForCall == DLL_PROCESS_ATTACH) 
    {
        HANDLE hThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Thread, hModule, 0, nullptr);

        if (hThread)
        {
            CloseHandle(hThread);
        }
    }

    return TRUE;
}

