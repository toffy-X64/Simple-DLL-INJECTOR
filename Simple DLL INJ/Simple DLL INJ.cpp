// Simple DLL INJ.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 31.12.2023 - 20:51
// BY TOFFY

#include "Inject.h"


int main()
{
	std::string proc_name;
	std::cout << "Enter proc name: ";
	std::cin >> proc_name;


	HANDLE procHandle = GetProcessHandleByName( proc_name.c_str() );
	if (procHandle) {
		InjectDLL(procHandle, "DLL PATH.dll");
	}
	else {
		MessageBoxA(0, "OpenProcess - ERROR", "[ERROR]", MB_OK | MB_ICONQUESTION);
	}
}
