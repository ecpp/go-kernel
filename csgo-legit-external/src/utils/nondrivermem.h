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

	
}

