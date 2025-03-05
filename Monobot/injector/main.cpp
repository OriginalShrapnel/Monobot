#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

DWORD GetProcessID(const char* processName)
{
    DWORD processID = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnap, &procEntry))
    {
        do
        {
            if (!_stricmp(procEntry.szExeFile, processName))
            {
                processID = procEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(hSnap, &procEntry));
    }
    CloseHandle(hSnap);
    return processID;
}

bool InjectDLL(DWORD processID, const char* dllPath)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
    if (!hProcess) return false;

    LPVOID allocMem = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!allocMem) return false;

    WriteProcessMemory(hProcess, allocMem, dllPath, strlen(dllPath) + 1, NULL);

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"), allocMem, 0, NULL);
    if (!hThread) return false;

    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}

int main()
{
    const char* processName = "hl2.exe";  // Garry's Mod
    const char* dllPath = "C:\\path\\to\\Monobot.dll"; // Update this!

    DWORD processID = GetProcessID(processName);
    if (processID == 0)
    {
        std::cout << "Garry's Mod not found!" << std::endl;
        return 1;
    }

    if (InjectDLL(processID, dllPath))
    {
        std::cout << "DLL Injected successfully!" << std::endl;
    }
    else
    {
        std::cout << "DLL Injection failed!" << std::endl;
    }

    return 0;
}
