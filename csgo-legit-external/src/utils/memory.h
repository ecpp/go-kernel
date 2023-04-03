#pragma once
#include <Windows.h>
#include <cstdint>
#include <string>
#include <Psapi.h>
#include <stdexcept>
#include <TlHelp32.h>
#include <iostream>

//class MemoryEditor
//{
//private:
//    DWORD processId;
//    HANDLE processHandle;
//
//public:
//    MemoryEditor(const std::string& processName)
//    {
//        processId = 0;
//        processHandle = nullptr;
//
//        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//        PROCESSENTRY32 entry;
//        entry.dwSize = sizeof(entry);
//
//        if (!Process32First(snapshot, &entry))
//        {
//            CloseHandle(snapshot);
//            throw std::runtime_error("Failed to enumerate processes");
//        }
//
//        do
//        {
//            if (processName == entry.szExeFile)
//            {
//                processId = entry.th32ProcessID;
//                processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
//                break;
//            }
//        } while (Process32Next(snapshot, &entry));
//
//        CloseHandle(snapshot);
//
//        if (!processHandle)
//        {
//            std::cout << "Failed to find csgo.exe'" << std::endl;
//            throw std::runtime_error("Failed to find process");
//        
//        }
//    }
//
//    // Destructor that frees the opened handle
//    ~MemoryEditor()
//    {
//        if (processHandle)
//            CloseHandle(processHandle);
//    }
//
//    const DWORD GetProcessId() const noexcept {
//        return processId;
//    }
//
//    bool IsProcessOpen() const noexcept {
//        return processHandle != nullptr;
//    }
//
//    // Returns the base address of a module by name
//    const std::uintptr_t GetModuleAddress(const std::string_view moduleName) const noexcept
//    {
//        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
//        MODULEENTRY32 entry;
//        entry.dwSize = sizeof(entry);
//
//        uintptr_t result = 0;
//
//        if (!Module32First(snapshot, &entry))
//        {
//            CloseHandle(snapshot);
//            throw std::runtime_error("Failed to enumerate modules");
//        }
//
//        do
//        {
//            if (moduleName == entry.szModule)
//            {
//                result = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
//                break;
//            }
//        } while (Module32Next(snapshot, &entry));
//
//        CloseHandle(snapshot);
//
//        if (!result)
//        {
//            throw std::runtime_error("Failed to find module");
//        }
//
//        return result;
//    }
//
//    // Read process memory
//    template <typename T>
//    constexpr const T Read(const std::uintptr_t& address) const noexcept
//    {
//        T value = { };
//        ::ReadProcessMemory(processHandle, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
//        return value;
//    }
//
//    // Write process memory
//    template <typename T>
//    constexpr void Write(const std::uintptr_t& address, const T& value) const noexcept
//    {
//        ::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
//    }
//};

