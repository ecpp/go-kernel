#pragma once

#define _CRT_SECURE_NO_WARNINGS



#include <Windows.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <tchar.h>
#include <Psapi.h>

#define NT_SUCCESS(x) ((x) >= 0)
#define in_range(x, a, b) (x >= a && x <= b)
#define get_bits(x) (in_range(x, '0', '9') ? (x - '0') : ((x & (~0x20)) - 'A' + 0xa))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))
namespace nonDriverMem {

	bool compare_memory(const byte* data, const char* pattern)
	{
		for (; *pattern; *pattern != ' ' ? ++data : data, ++pattern)
		{
			if (*pattern == ' ' || *pattern == '?') continue;
			if (*data != get_byte(pattern)) return false;
			++pattern;
		}
		return true;
	}

	DWORD patternScan(ULONG base, ULONG size, const char* pattern, int offset, int extra, bool relative, bool subtract)
	{
		DWORD address = 0;

		byte* data = new byte[size];
		Driver::rpm_pattern<byte>(base, data, size);
		for (DWORD i = 0x1000; i < size; i++)
		{
			if (compare_memory((const byte*)(data + i), pattern))
			{
				address = base + i + offset;
				if (relative) { address = Driver::rpm<DWORD>(address); }
				if (subtract) { address -= base; }
				address += extra;
				break;
			}
		}

		delete[] data;
		return address;
	}
	
	HANDLE get_process_handle()
	{
		HANDLE h = 0;
		DWORD pid = 0;
		HWND hWnd = FindWindow(0, _T(globals::windowName));
		if (hWnd == 0) {
			std::cout << "get_process_handle: FindWindow failed" << std::endl;
			return h;
		}
		else {
			std::cout << "get_process_handle: Found window" << std::endl;
		}
		GetWindowThreadProcessId(hWnd, &pid);
		h = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
		if (h == 0) {
			std::cout << "get_process_handle: OpenProcess failed" << std::endl;
			return h;
		}
		else {
			std::cout << "get_process_handle: Got handle" << std::endl;
		}

		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		MODULEENTRY32 me32;

		// Take a snapshot of all modules in the specified process.
		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, pid);
		if (hModuleSnap == INVALID_HANDLE_VALUE)
		{
			std::cout << "CreateToolhelp32Snapshot failed" << std::endl;
			return h;
		}

		// Set the size of the structure before using it.
		me32.dwSize = sizeof(MODULEENTRY32);

		// Retrieve information about the first module,
		// and exit if unsuccessful
		if (!Module32First(hModuleSnap, &me32))
		{
			std::cout << "Module32First failed" << std::endl;  // show cause of failure
			CloseHandle(hModuleSnap);     // clean the snapshot object
			return h;
		}

		// Now walk the module list of the process,
		// and display information about each module
		do
		{
			//check if name is engine.dll
			if (strcmp(me32.szModule, "engine.dll") == 0) {
				globals::enginesize = me32.modBaseSize;
			}
			
		} while (Module32Next(hModuleSnap, &me32));

		CloseHandle(hModuleSnap);
		return h;
	}

	
}

