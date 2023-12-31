#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <tlhelp32.h>

bool InjectDLL(HANDLE hProc, const std::string& DLL_Path)
{
    if (hProc == nullptr || DLL_Path.empty()) {
        return false;
    }

    ////////////////////////////////////////////
    long dll_size = DLL_Path.length() + 1;
    LPVOID MyAlloc = VirtualAllocEx(hProc, NULL, dll_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (MyAlloc == NULL) return false;
    int IsWriteOK = WriteProcessMemory(hProc, MyAlloc, DLL_Path.c_str(), dll_size, 0);
    if (IsWriteOK == 0) return false;
    DWORD dWord = NULL;
#pragma warning(suppress: 6387)
    LPTHREAD_START_ROUTINE addrLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibraryA("kernel32.dll"), "LoadLibraryA");
    HANDLE ThreadReturn = CreateRemoteThread(hProc, NULL, 0, addrLoadLibrary, MyAlloc, 0, &dWord);
    if (ThreadReturn == NULL) return false;
    if ((hProc != NULL) && (MyAlloc != NULL) && (IsWriteOK != ERROR_INVALID_HANDLE) && (ThreadReturn != NULL)) {
        MessageBoxA(0, "DLL SUCCESSFULLY EXECUTED", "[EXECUTING]", MB_OK);
        return true;
    }
    return false;
}

HANDLE GetProcessHandleByName(PCSTR name) {
    DWORD pid = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    if (Process32First(snapshot, &process)) {
        do {
            std::wstring wProcessName(process.szExeFile);
            std::string convertedProcessName(wProcessName.begin(), wProcessName.end());

            if (convertedProcessName == std::string(name)) {
                pid = process.th32ProcessID;
                break;
            }
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0) {
        return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    return NULL;
}