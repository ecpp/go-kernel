#include "memory.h"
#include "imports.h"

NTSTATUS hook_handler(PVOID called_param)
{
	COPY_MEMORY* m = (COPY_MEMORY*)called_param;

	if (m->get_pid) {
		m->pid = memory::get_process_id(m->process_name);
	}
	else if (m->base) {
		PEPROCESS process;
		if (NT_SUCCESS(PsLookupProcessByProcessId(m->pid, &process))) {
			m->buffer = (void*)memory::get_module_base_x64(process);
		}
	}
	else if (m->peb) {
		PEPROCESS process;
		if (NT_SUCCESS(PsLookupProcessByProcessId(m->pid, &process))) {
			m->buffer = (void*)PsGetProcessPeb(process);
		}
	}
	else if (m->get_client) {
		PEPROCESS process;
		if (NT_SUCCESS(PsLookupProcessByProcessId(m->pid, &process))) {
			UNICODE_STRING DLLName;
			RtlInitUnicodeString(&DLLName, L"client.dll");
			ULONG64 BaseAddr = memory::GetModuleBasex64(process, DLLName, false);
			m->buffer = (void*)BaseAddr;
		}
	}
	else if (m->get_engine) {
		PEPROCESS process;
		if (NT_SUCCESS(PsLookupProcessByProcessId(m->pid, &process))) {
			UNICODE_STRING DLLName;
			RtlInitUnicodeString(&DLLName, L"engine.dll");
			ULONG64 BaseAddr = memory::GetModuleBasex64(process, DLLName, false);
			m->buffer = (void*)BaseAddr;
		}
	}
	else if (m->get_engine_size) {
		PEPROCESS process;
		if (NT_SUCCESS(PsLookupProcessByProcessId(m->pid, &process))) {
			UNICODE_STRING DLLName;
			RtlInitUnicodeString(&DLLName, L"engine.dll");
			ULONG64 BaseAddr = memory::GetModuleBasex64(process, DLLName, true);
			m->size = BaseAddr;
		}
	}
	else if (m->get_client_size) {
		PEPROCESS process;
		if (NT_SUCCESS(PsLookupProcessByProcessId(m->pid, &process))) {
			UNICODE_STRING DLLName;
			RtlInitUnicodeString(&DLLName, L"client.dll");
			ULONG64 BaseAddr = memory::GetModuleBasex64(process, DLLName, true);
			m->size = BaseAddr;
		}
	}
	else if (m->read) {
		//m->buffer = (void*)memory::read2(m->pid, (PVOID)m->address, m->buffer, m->size);
		memory::read_kernel_memory(m->pid, (PVOID)m->address, m->buffer, m->size);
	}
	else if (m->write) {
		memory::write_kernel_memory(m->pid, m->buffer, (PVOID)m->address, m->size);
	}

	return STATUS_SUCCESS;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path)
{
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);
	DbgPrintEx(0, 0, "GoKernel: Loaded!\n");
	if (memory::call_kernel_function(&hook_handler)) {
		DbgPrintEx(0, 0, "GoKernel: Driver Ready!\n");
	}
	else {
		DbgPrintEx(0, 0, "GoKernel: failed to hook function.\n");
	}
	return STATUS_SUCCESS;
}