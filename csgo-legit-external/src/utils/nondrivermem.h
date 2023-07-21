#pragma once

#define _CRT_SECURE_NO_WARNINGS



#include <Windows.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <tchar.h>
#include <Psapi.h>

#define NT_SUCCESS(x) ((x) >= 0)

namespace nonDriverMem {
	typedef NTSTATUS(NTAPI* _NtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, LPCVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);
	_NtWriteVirtualMemory NtWriteVirtualMemory = (_NtWriteVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtWriteVirtualMemory");
	typedef NTSTATUS(NTAPI* _NtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesRead);
	_NtReadVirtualMemory NtReadVirtualMemory = (_NtReadVirtualMemory)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtReadVirtualMemory");

	HANDLE hProcess;

	template <class dataType>
	void wvmb(DWORD addressToWrite, ULONG num, dataType* valToWrite)
	{
		DWORD oldProtect = 0;  NTSTATUS status = 0;
		VirtualProtectEx(hProcess, (PVOID)addressToWrite, num, PAGE_EXECUTE_READWRITE, &oldProtect);
		if (!NT_SUCCESS(status = NtWriteVirtualMemory(hProcess, (PVOID)addressToWrite, valToWrite, num, NULL)))
			std::cout << "wvmb error" << std::endl;
		VirtualProtectEx(hProcess, (PVOID)addressToWrite, num, oldProtect, NULL);
	}

	template <class dataType>
	void wvmb(DWORD addressToWrite, dataType* valToWrite)
	{
		DWORD oldProtect = 0;  NTSTATUS status = 0;
		VirtualProtectEx(hProcess, (PVOID)addressToWrite, sizeof(dataType), PAGE_EXECUTE_READWRITE, &oldProtect);
		if (!NT_SUCCESS(status = NtWriteVirtualMemory(hProcess, (PVOID)addressToWrite, valToWrite, sizeof(dataType), NULL)))
			std::cout << "wvmb error" << std::endl;
		VirtualProtectEx(hProcess, (PVOID)addressToWrite, sizeof(dataType), oldProtect, NULL);
	}

	template <class dataType>
	void wvm(DWORD addressToWrite, dataType valToWrite)
	{
		DWORD oldProtect = 0;  NTSTATUS status = 0;
		VirtualProtectEx(hProcess, (PVOID)addressToWrite, sizeof(dataType), PAGE_EXECUTE_READWRITE, &oldProtect);
		if (!NT_SUCCESS(status = NtWriteVirtualMemory(hProcess, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), NULL)))
			std::cout << "wvmb error" << std::endl;
		VirtualProtectEx(hProcess, (PVOID)addressToWrite, sizeof(dataType), oldProtect, NULL);
	}

	template <class dataType>
	dataType rvm(DWORD addressToRead)
	{
		dataType rpmBuffer;
		DWORD oldProtect = 0;
		NtReadVirtualMemory(hProcess, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);
		return rpmBuffer;
	}

	template <class dataType>
	void rvm(DWORD addressToRead, dataType* valToRead)
	{
		DWORD oldProtect = 0;  NTSTATUS status = 0;
		VirtualProtectEx(hProcess, (PVOID)addressToRead, sizeof(dataType), PAGE_EXECUTE_READWRITE, &oldProtect);
		status = NtReadVirtualMemory(hProcess, PVOID(addressToRead), valToRead, sizeof(dataType), 0);
		VirtualProtectEx(hProcess, (PVOID)addressToRead, sizeof(dataType), oldProtect, NULL);
	}

	PVOID SpyInjectAndJump(void* from, void* to, int nops)
	{
		size_t funcsize = 0;
		unsigned char* p = (unsigned char*)from;
		for (funcsize = 0; funcsize < 512; ++funcsize)
			if (p[funcsize] == (int)0xCC && p[funcsize + 1] == (int)0xCC && p[funcsize + 2] == (int)0xCC && p[funcsize + 3] == (int)0xCC)
				break;

		LPVOID Cave = VirtualAllocEx(hProcess, NULL, funcsize + 8, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

#ifdef DEBUG
		cout << AY_OBFUSCATE("Injecting our function (addr 0x") << hex << from << AY_OBFUSCATE(", size 0x") << funcsize << AY_OBFUSCATE(") to cave (addr 0x") << Cave << ")" << endl;
#endif
		WriteProcessMemory(hProcess, Cave, p, funcsize, 0);

		DWORD dBack = (DWORD)to + 0x5;
		BYTE* pushbytes = new BYTE[6];
		pushbytes[0] = 0x68;
		pushbytes[5] = 0xC3;
		memcpy(pushbytes + 1, &dBack, sizeof(DWORD));
		wvmb((DWORD)Cave + funcsize, 6, pushbytes);

		delete[] pushbytes;

		DWORD dCave = 0xFFFFFFFF - ((DWORD)to + 0x4 - (DWORD)Cave);
		BYTE* jumpbytes = new BYTE[5 + nops];
		jumpbytes[0] = 0xE9;
		memcpy(jumpbytes + 1, &dCave, sizeof(DWORD));
		for (int i = 0; i < nops; i++)
			jumpbytes[5 + i] = 0x90;
		wvmb(DWORD(to), 5 + nops, jumpbytes);

		delete[] jumpbytes;
		return Cave;
	}


	bool MemoryCompare(const BYTE* bData, const BYTE* bMask, const char* szMask) {
		for (; *szMask; ++szMask, ++bData, ++bMask) {
			if (*szMask == 'x' && *bData != *bMask) {
				return false;
			}
		}
		return (*szMask == NULL);
	}

	DWORD SpyPatternScan(BYTE* data, DWORD size, const char* pattern)
	{
		BYTE* patternbytes = new BYTE[255];
		char* maskbytes = new char[255];
		int i = 0;  unsigned uchr;

		std::string s = std::string(pattern);
		std::string delimiter = " ";

		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			maskbytes[i] = (token == "?" ? '?' : 'x');
			sscanf_s(token.c_str(), "%2x", &uchr);  // conversion
			patternbytes[i] = uchr;  // save as char
			i++;
			s.erase(0, pos + delimiter.length());
		}

		maskbytes[i] = 0x78;  maskbytes[i + 1] = 0x0;
		sscanf_s(s.c_str(), "%2x", &uchr);  // conversion
		patternbytes[i] = uchr;  // save as char

		for (DWORD i = 0; i < size; i++)
		{
			if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)patternbytes, maskbytes)) {

				std::cout << "SpyPatternScan: Found at " << std::hex << (DWORD)data + i << std::endl;
				return i;
			}
		}
		std::cout << "SpyPatternScan: Nothing found" << std::endl;
		return NULL;
	}


	DWORD FindSignature(DWORD start, DWORD size, const char* sig, const char* mask)
	{
		BYTE* data = new BYTE[size];
		SIZE_T bytesRead;

		ReadProcessMemory(hProcess, (LPVOID)start, data, size, &bytesRead);

		for (DWORD i = 0; i < size; i++)
		{
			if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {
				delete[] data;
				std::cout << "FindSignature: Found at " << std::hex << start + i << std::endl;
				return start + i;
			}
		}
		delete[] data;
		std::cout << "FindSignature: Nothing found" << std::endl;
		return NULL;
	}


	DWORD FindSignatureLocal(BYTE* data, DWORD size, const char* sig, const char* mask, const char* name = "")
	{
		for (DWORD i = 0; i < size; i++)
		{
			if (MemoryCompare((const BYTE*)(data + i), (const BYTE*)sig, mask)) {

				std::cout << "FindSignatureLocal: Found at " << std::hex << (DWORD)data + i << " (" << name << ")" << std::endl;
				return i;
			}
		}
		std::cout << "FindSignatureLocal: Nothing found" << std::endl;
		return NULL;
	}

	std::string folder;
	DWORD engine_dll, engine_dll_size, vstdlib_dll, vstdlib_dll_size, client_dll, client_dll_size;
	DWORD PID = 0;
	char tWindowName[256] = "Counter-Strike: Global Offensive - Direct3D 9";
	HANDLE get_process_handle()
	{
		HANDLE h = 0;
		DWORD pid = 0;
		HWND hWnd = FindWindow(0, _T(tWindowName));
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
		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
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
			std::cout << "Module name: " << me32.szModule << std::endl;
			if (_tcsstr(me32.szModule, _T("engine.dll")) != NULL) {
				std::cout << "engine.dll base: " << std::hex << me32.modBaseAddr << ", its size " << std::hex << me32.modBaseSize << std::endl;
			}
		} while (Module32Next(hModuleSnap, &me32));

		CloseHandle(hModuleSnap);
		return h;
	}

	class CRecvProp;
	class CRecvTable
	{
	public:
		const char* GetTableName()
		{
			char pszTabelName[128];
			rvm<char[128]>(rvm<DWORD>((DWORD)this + 0xC), &pszTabelName);
			return pszTabelName;
		}

		int GetMaxProp()
		{
			return rvm<int>((DWORD)this + 0x4);
		}

		CRecvProp* GetProp(int iIndex)
		{
			return (CRecvProp*)(rvm<DWORD>((DWORD)this) + 0x3C * iIndex);
		}
	};

	class CRecvProp
	{
	public:
		const char* GetVarName()
		{
			char pszVarName[128];
			rvm<char[128]>(rvm<DWORD>((DWORD)this), &pszVarName);
			return pszVarName;
		}

		int GetOffset()
		{
			return rvm<int>((DWORD)this + 0x2C);
		}

		CRecvTable* GetDataTable()
		{
			return rvm<CRecvTable*>((DWORD)this + 0x28);
		}
	};

	class ClientClass
	{
	public:
		const char* GetNetworkName()
		{
			char pszNetworkName[128];
			rvm<char[128]>(rvm<DWORD>((DWORD)this + 0x8), &pszNetworkName);
			return pszNetworkName;
		}

		ClientClass* GetNextClass()
		{
			return rvm<ClientClass*>((DWORD)this + 0x10);
		}

		CRecvTable* GetTable()
		{
			return rvm<CRecvTable*>((DWORD)this + 0xC);
		}
	};



#include <tlhelp32.h>
	void Suspend(bool b)
	{
		HANDLE        hThreadSnap = NULL;
		BOOL          bRet = FALSE;
		THREADENTRY32 te32 = { 0 };
		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
#ifdef DEBUG
		if (hThreadSnap == INVALID_HANDLE_VALUE)
			cout << AY_OBFUSCATE("INVALID_HANDLE_VALUE\n");
#endif
		te32.dwSize = sizeof(THREADENTRY32);
		if (Thread32First(hThreadSnap, &te32))
		{
			do
			{
				if (te32.th32OwnerProcessID == PID)
				{
					HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
					if (!b)
					{
#ifdef DEBUG
						cout << _T(AY_OBFUSCATE("Resuming Thread 0x")) << cout.setf(ios_base::hex) << te32.th32ThreadID << endl;
#endif
						ResumeThread(hThread);
					}
					else
					{
#ifdef DEBUG
						cout << _T(AY_OBFUSCATE("Suspending Thread 0x")) << cout.setf(ios_base::hex) << te32.th32ThreadID << endl;
#endif
						SuspendThread(hThread);
					}
					CloseHandle(hThread);
				}
			} while (Thread32Next(hThreadSnap, &te32));
			bRet = TRUE;
		}
		else
			bRet = FALSE;
		CloseHandle(hThreadSnap);
#ifdef DEBUG
		cout << AY_OBFUSCATE("Suspend returned ") << bRet << endl;
#endif
	}
}

