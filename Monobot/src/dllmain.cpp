#include <windows.h>
#include <iostream>

DWORD WINAPI MainThread(LPVOID lpParam)
{
    MessageBoxA(NULL, "Monobot Injected Successfully!", "Monobot Made by OGShrapnel", MB_OK | MB_ICONINFORMATION);

    // Main loop (for ESP, Aim Assist, etc. later)
    while (true)
    {
        Sleep(100);
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
    }
    return TRUE;
}
