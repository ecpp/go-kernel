#pragma once
#include "../../globals.h"
#include <cstdint>
#include <memory>
#include <string_view>
#include <TlHelp32.h>
#include <mutex>
#include <iostream>


typedef struct _COPY_MEMORY {
	void* buffer;
	ULONG64		address;
	ULONG		size;
	HANDLE		pid;
	bool		get_pid;
	bool		base;
	bool		peb;
	bool		read;
	bool		write;
	bool		get_client;
	bool		get_engine;
	bool		get_engine_size;
	bool		get_client_size;
	const char* module_name;
	const char* process_name;
}COPY_MEMORY;

namespace Driver
{
	static std::once_flag flag;

	template<typename ... A>
	uint64_t call_hook(const A ... arguments)
	{
		std::call_once(flag, [] { LoadLibrary(AY_OBFUSCATE("user32.dll")); });
		void* control_function = GetProcAddress(LoadLibrary(AY_OBFUSCATE("win32u.dll")), AY_OBFUSCATE("NtOpenCompositionSurfaceSectionInfo")); //NtOpenCompositionSurfaceSectionInfo

		const auto control = static_cast<uint64_t(__stdcall*)(A...)>(control_function);
		return control(arguments ...);
	}

	static HANDLE get_process_id(const char* process_name) {
		COPY_MEMORY m{};
		m.get_pid = true;
		m.process_name = process_name;
		call_hook(&m);
		return m.pid;
	}

	static uintptr_t get_client(HANDLE pID)
	{
		COPY_MEMORY m{};
		m.get_client = true;
		m.pid = pID;
		call_hook(&m);
		return (uintptr_t)m.buffer;
	}

	static uintptr_t get_engine(HANDLE pID)
	{
		COPY_MEMORY m{};
		m.get_engine = true;
		m.pid = pID;
		call_hook(&m);
		return (uintptr_t)m.buffer;
	}


	static ULONG get_engine_size(HANDLE pID)
	{
		COPY_MEMORY m{};
		m.get_engine_size = true;
		m.pid = pID;
		call_hook(&m);
		return (ULONG)m.size;
	}

	static ULONG get_client_size(HANDLE pID)
	{
		COPY_MEMORY m{};
		m.get_client_size = true;
		m.pid = pID;
		call_hook(&m);
		return (ULONG)m.size;
	}

	static uintptr_t GetModuleBase(HANDLE pID, const char* modName)
	{
		COPY_MEMORY m{};
		m.base = true;
		m.pid = pID;
		m.module_name = modName;
		call_hook(&m);

		return (uintptr_t)m.buffer;
	}

	template<typename type>
	type rpm(uintptr_t readAddress)
	{
		type buffer{};

		COPY_MEMORY m{};
		m.read = true;
		m.pid = globals::processID;
		m.address = readAddress;
		m.buffer = &buffer;
		m.size = sizeof(type);

		call_hook(&m);
		return buffer;
	}

	template<typename type>
	void rpm_pattern(uintptr_t readAddress, type* buffer, size_t size)
	{
		COPY_MEMORY m{};
		m.read = true;
		m.pid = globals::processID;
		m.address = readAddress;
		m.buffer = buffer;
		m.size = size * sizeof(type);

		call_hook(&m);
	}


	template<typename type>
	void wpm(UINT_PTR writeAddress, type value)
	{
		COPY_MEMORY m{};
		m.write = true;
		m.pid = globals::processID;
		m.address = writeAddress;
		m.buffer = &value;
		m.size = sizeof(value);
		call_hook(&m);
	}

};