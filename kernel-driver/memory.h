#pragma once
#include "imports.h"
#include <ntstrsafe.h>

typedef struct _COPY_MEMORY {
	void* buffer;
	ULONG64		address;
	ULONG64		size;
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

namespace memory {
	PVOID get_system_module_base(const char* module_name) {
		ULONG bytes = 0;
		NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

		if (!bytes)
			return 0;
		PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x454E4F45); // 'ENON'

		status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

		if (!NT_SUCCESS(status))
			return 0;
		PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
		PVOID module_base = 0, module_size = 0;

		for (ULONG i = 0; i < modules->NumberOfModules; i++)
		{
			if (strcmp((char*)module[i].FullPathName, module_name) == 0)
			{
				module_base = module[i].ImageBase;
				module_size = (PVOID)module[i].ImageSize;
				break;
			}
		}

		if (modules)
			ExFreePoolWithTag(modules, 0);
		if (module_base <= (void*)0)
			return 0;
		return module_base;
	}

	PVOID get_client_dll(PEPROCESS proc, const char* module_name) {
		DbgPrint("requested: %s", module_name);
		PVOID module_base = NULL;
		ULONG size = 0;
		ZwQuerySystemInformation(SystemModuleInformation, &module_base, size, &size);
		module_base = ExAllocatePool(NonPagedPool, size);
		if (module_base == NULL)
			return NULL;
		if (!NT_SUCCESS(ZwQuerySystemInformation(SystemModuleInformation, module_base, size, &size)))
		{
			ExFreePool(module_base);
			return NULL;
		}
		PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)module_base;
		for (ULONG i = 0; i < modules->NumberOfModules; i++)
		{
			PRTL_PROCESS_MODULE_INFORMATION module = &modules->Modules[i];
			
			//DbgPrint("found this: %s", module->FullPathName);
			if (strstr((char*)module->FullPathName, module_name) || strstr((char*)module->FullPathName, "client"))
			{
				ExFreePool(module_base);
				return module->ImageBase;
			}
		}
		ExFreePool(module_base);
		return NULL;

	}

	ULONG64 get_module_base_x64(PEPROCESS proc) {
		return (ULONG64)PsGetProcessSectionBaseAddress(proc);
	}

	ULONG64 GetModuleBasex64(PEPROCESS proc, UNICODE_STRING module_name, BOOL get_size) {
		PCWSTR modName = module_name.Buffer;
		UNICODE_STRING moduleName;
		RtlInitUnicodeString(&moduleName, modName);

		PPEB pPeb = (PPEB)PsGetProcessPeb(proc); // get Process PEB, function is unexported and undoc

		if (!pPeb) {
			return 0; // failed
		}

		KAPC_STATE state;

		KeStackAttachProcess(proc, &state);

		PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)pPeb->Ldr;

		if (!pLdr) {
			KeUnstackDetachProcess(&state);
			return 0; // failed
		}

		UNICODE_STRING name;

		// loop the linked list
		for(PLIST_ENTRY listEntry = (PLIST_ENTRY)pLdr->InLoadOrderModuleList.Flink;
        listEntry != &pLdr->InLoadOrderModuleList;
        listEntry = (PLIST_ENTRY)listEntry->Flink) {
			PLDR_DATA_TABLE_ENTRY pEntry =
				CONTAINING_RECORD(listEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			if (RtlCompareUnicodeString(&pEntry->BaseDllName, &moduleName, TRUE) ==
				0) {
				ULONG64 baseAddr = (ULONG64)pEntry->DllBase;
				ULONG64 size = (ULONG64)pEntry->SizeOfImage;
				KeUnstackDetachProcess(&state);

				if (get_size)
					return size;

				return baseAddr;
			}
		}

		KeUnstackDetachProcess(&state);

		return 0; // failed
	}



	

	HANDLE get_process_id(const char* process_name) {
		ULONG buffer_size = 0;
		ZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &buffer_size);

		auto buffer = ExAllocatePoolWithTag(NonPagedPool, buffer_size, 'mder');
		if (!buffer) {
			DbgPrintEx(0, 0, "failed to allocate pool (get_process_id)");
			return 0;
		}

		ANSI_STRING process_name_ansi = {};
		UNICODE_STRING process_name_unicode = {};
		RtlInitAnsiString(&process_name_ansi, process_name);
		if (!NT_SUCCESS(RtlAnsiStringToUnicodeString(&process_name_unicode, &process_name_ansi, TRUE))) {
			DbgPrintEx(0, 0, "failed to convert string (get_process_id)");
			RtlFreeUnicodeString(&process_name_unicode);
			return 0;
		}

		auto process_info = (PSYSTEM_PROCESS_INFO)buffer;
		if (NT_SUCCESS(ZwQuerySystemInformation(SystemProcessInformation, process_info, buffer_size, NULL))) {
			while (process_info->NextEntryOffset) {
				if (!RtlCompareUnicodeString(&process_name_unicode, &process_info->ImageName, true)) {
					DbgPrintEx(0, 0, "process name: %wZ | process ID: %d\n", process_info->ImageName, process_info->UniqueProcessId);
					RtlFreeUnicodeString(&process_name_unicode);
					return process_info->UniqueProcessId;
				}
				process_info = (PSYSTEM_PROCESS_INFO)((BYTE*)process_info + process_info->NextEntryOffset);
			}
		}
		else {
			ExFreePoolWithTag(buffer, 'mder');
			return 0;
		}
	}

	PVOID get_system_module_export(const char* module_name, LPCSTR routine_name)
	{
		PVOID lpModule = memory::get_system_module_base(module_name);

		if (!lpModule)
			return NULL;

		return RtlFindExportedRoutineByName(lpModule, routine_name);
	}

	bool write_to_read_only_memory(void* address, void* buffer, size_t size) {

		PMDL Mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);

		if (!Mdl)
			return false;

		// Locking and mapping memory with RW-rights:
		MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
		PVOID Mapping = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);
		MmProtectMdlSystemAddress(Mdl, PAGE_READWRITE);

		// Write your buffer to mapping:
		RtlCopyMemory(Mapping, buffer, size);

		// Resources freeing:
		MmUnmapLockedPages(Mapping, Mdl);
		MmUnlockPages(Mdl);
		IoFreeMdl(Mdl);

		return true;
	}

	bool call_kernel_function(void* kernel_function_address) {
		if (!kernel_function_address)
			return false;

		//Used Function and Shellcode from Null's tutorials, this is meant for you to REPLACE, this is DETECTED
		PVOID* dxgk_routine
			= reinterpret_cast<PVOID*>(memory::get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", "NtOpenCompositionSurfaceSectionInfo"));

		if (!dxgk_routine) {
			return false;
		}

		//What is this? - Shellcode https://www.exploit-db.com/docs/english/13019-shell-code-for-beginners.pdf
		BYTE dxgk_original[] = { 0x8B, 0x04, 0x24, 0x89, 0x41, 0x44, 0xC7, 0x41, 0x30, 0x0F, 0x00, 0x10 };

		BYTE shell_code_start[]
		{
			0x48, 0xB8 // 0x48 is mov, 0xBB is rax; then our function address
		};

		BYTE shell_code_end[]
		{
			0xFF, 0xE0, // jmp rax
			0xCC
		};

		RtlSecureZeroMemory(&dxgk_original, sizeof(dxgk_original));
		memcpy((PVOID)((ULONG_PTR)dxgk_original), &shell_code_start, sizeof(shell_code_start));
		uintptr_t test_address = reinterpret_cast<uintptr_t>(kernel_function_address);
		memcpy((PVOID)((ULONG_PTR)dxgk_original + sizeof(shell_code_start)), &test_address, sizeof(void*));
		memcpy((PVOID)((ULONG_PTR)dxgk_original + sizeof(shell_code_start) + sizeof(void*)), &shell_code_end, sizeof(shell_code_end));
		write_to_read_only_memory(dxgk_routine, &dxgk_original, sizeof(dxgk_original));

		return true;
	}

	bool read_kernel_memory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size) {
		if (!address || !buffer || !size)
			return false;
		SIZE_T bytes = 0;
		PEPROCESS process;
		if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &process))) {
			DbgPrintEx(0, 0, "process lookup failed (read)");
			return false;
		}
		return MmCopyVirtualMemory(process, address, PsGetCurrentProcess(), buffer, size, KernelMode, &bytes) == STATUS_SUCCESS;
	}


	bool write_kernel_memory(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size) {
		if (!address || !buffer || !size)
			return false;
		SIZE_T bytes = 0;
		PEPROCESS process;
		if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &process))) {
			DbgPrintEx(0, 0, "process lookup failed (write)");
			return false;
		}
		return MmCopyVirtualMemory(PsGetCurrentProcess(), address, process, buffer, size, KernelMode, &bytes) == STATUS_SUCCESS;
	}

	PVOID read2(HANDLE pid, PVOID address, PVOID buffer, SIZE_T size) {
		if (!address || !buffer || !size)
			return (void*)1;
		DbgPrint("db1");
		SIZE_T bytes = 0;
		PEPROCESS process;
		if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &process))) {
			DbgPrintEx(0, 0, "process lookup failed (read)");
			return (void*)2;
		}
		MmCopyVirtualMemory(process, address, PsGetCurrentProcess(), buffer, size, KernelMode, &bytes);
		return buffer;
	}

	NTSTATUS protect_virtual_memory(HANDLE pid, PVOID address, ULONG size, ULONG protection, ULONG& protection_out)
	{
		if (!pid || !address || !size || !protection)
			return STATUS_INVALID_PARAMETER;

		NTSTATUS status = STATUS_SUCCESS;
		PEPROCESS target_process = nullptr;

		if (!NT_SUCCESS(PsLookupProcessByProcessId(reinterpret_cast<HANDLE>(pid), &target_process)))
		{
			return STATUS_NOT_FOUND;
		}

		//PVOID address = reinterpret_cast<PVOID>( memory_struct->address );
		//ULONG size = (ULONG)( memory_struct->size );
		//ULONG protection = memory_struct->protection;
		ULONG protection_old = 0;

		KAPC_STATE state;
		KeStackAttachProcess(target_process, &state);

		status = ZwProtectVirtualMemory(NtCurrentProcess(), &address, &size, protection, &protection_old);

		KeUnstackDetachProcess(&state);

		if (NT_SUCCESS(status))
			protection_out = protection_old;

		ObDereferenceObject(target_process);
		return status;
	}
}