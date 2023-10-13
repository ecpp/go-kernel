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


	//inline const DWORD BLOCKMAXSIZE = 409600;


	//int GetSignatureArray(const std::string& Signature, std::vector<WORD>& SignatureArray)
	//{
	//	std::string Sig = Signature;
	//	Sig.erase(std::remove(Sig.begin(), Sig.end(), ' '), Sig.end());

	//	std::size_t Size = Sig.size();

	//	if (Size % 2 != 0)
	//		return 0;

	//	for (int i = 0; i < Size; i += 2)
	//	{
	//		std::string ByteString = Sig.substr(i, 2);
	//		WORD Byte = (ByteString == "??") ? 256 : std::stoi(ByteString, nullptr, 16);
	//		SignatureArray.push_back(Byte);
	//	}
	//	return SignatureArray.size();
	//}

	//void GetNextArray(std::vector<short>& NextArray, const std::vector<WORD>& SignatureArray)
	//{
	//	std::size_t Size = SignatureArray.size();
	//	for (int i = 0; i < Size; i++)
	//		NextArray[SignatureArray[i]] = i;
	//}

	//void SearchMemoryBlock(byte* MemoryBuffer, const std::vector<short>& NextArray, const std::vector<WORD>& SignatureArray, DWORD64 StartAddress, DWORD Size, std::vector<DWORD64>& ResultArray)
	//{
	//	if (!ProcessMgr.ReadMemory(StartAddress, *MemoryBuffer, Size))
	//		return;

	//	int SignatureLength = SignatureArray.size();

	//	for (int i = 0, j, k; i < Size;)
	//	{
	//		j = i; k = 0;

	//		for (; k < SignatureLength && j < Size && (SignatureArray[k] == MemoryBuffer[j] || SignatureArray[k] == 256); k++, j++);

	//		if (k == SignatureLength)
	//			ResultArray.push_back(StartAddress + i);

	//		if ((i + SignatureLength) >= Size)
	//			return;

	//		int Num = NextArray[MemoryBuffer[i + SignatureLength]];
	//		if (Num == -1)
	//			i += (SignatureLength - NextArray[256]);
	//		else
	//			i += (SignatureLength - Num);
	//	}
	//}

	//std::vector<DWORD64> SearchMemory(const std::string& Signature, DWORD64 StartAddress, DWORD64 EndAddress, int SearchNum)
	//{
	//	std::vector<DWORD64> ResultArray;
	//	std::vector<WORD> SignatureArray;
	//	std::vector<short> NextArray(260, -1);

	//	byte* MemoryBuffer = new byte[BLOCKMAXSIZE];

	//	if (GetSignatureArray(Signature, SignatureArray) <= 0)
	//		return ResultArray;

	//	GetNextArray(NextArray, SignatureArray);

	//	MEMORY_BASIC_INFORMATION mbi;
	//	int Count;
	//	while (VirtualQueryEx(hProcess, reinterpret_cast<LPCVOID>(StartAddress), &mbi, sizeof(mbi)) != 0)
	//	{
	//		Count = 0;
	//		auto BlockSize = mbi.RegionSize;

	//		while (BlockSize >= BLOCKMAXSIZE)
	//		{
	//			if (ResultArray.size() >= SearchNum)
	//				goto END;

	//			SearchMemoryBlock(MemoryBuffer, NextArray, SignatureArray, StartAddress + (BLOCKMAXSIZE * Count), BLOCKMAXSIZE, ResultArray);

	//			BlockSize -= BLOCKMAXSIZE;
	//			Count++;
	//		}

	//		SearchMemoryBlock(MemoryBuffer, NextArray, SignatureArray, StartAddress + (BLOCKMAXSIZE * Count), BlockSize, ResultArray);

	//		StartAddress += mbi.RegionSize;

	//		if (ResultArray.size() >= SearchNum || EndAddress != 0 && StartAddress > EndAddress)
	//			break;
	//	}

	//END:

	//	delete[] MemoryBuffer;
	//	return ResultArray;
	//}


	//ptrdiff_t convertToPtr(ULONG64 value)
	//{
	//	return static_cast<ptrdiff_t>(value);
	//}

	//DWORD64 SearchOffsets(std::string Signature, DWORD64 ModuleAddress)
	//{
	//	std::vector<DWORD64> TempAddressList;
	//	DWORD64 Address = 0;
	//	DWORD Offsets = 0;

	//	TempAddressList = ProcessMgr.SearchMemory(Signature, ModuleAddress, ModuleAddress + 0x4000000);

	//	if (TempAddressList.size() <= 0)
	//		return 0;

	//	if (!ProcessMgr.ReadMemory<DWORD>(TempAddressList.at(0) + 3, Offsets))
	//		return 0;

	//	Address = TempAddressList.at(0) + Offsets + 7;
	//	return Address;
	//}

	//bool UpdateOffsets()
	//{
	//	

	//	DWORD64 TempAddress = 0;

	//	TempAddress = SearchOffsets(offset::Signatures::EntityList, globals::client);
	//	if (TempAddress == 0)
	//		return false;

	//	offset::dwEntityList = TempAddress - globals::client;

	//	TempAddress = SearchOffsets(offset::Signatures::LocalPlayerController, globals::client);
	//	if (TempAddress == 0)
	//		return false;

	//	offset::dwLocalPlayerPawn = TempAddress - globals::client;

	//	TempAddress = SearchOffsets(offset::Signatures::ViewMatrix, globals::client);
	//	if (TempAddress == 0)
	//		return false;

	//	offset::dwViewMatrix = TempAddress - globals::client;

	//	TempAddress = SearchOffsets(offset::Signatures::ViewAngles, globals::client);
	//	if (TempAddress == 0)
	//		return false;
	//	if (!Driver::rpm<DWORD64>(TempAddress))
	//		return false;

	//	offset::dwClientState_ViewAngles = TempAddress + 0x4510 - globals::client;

	//	TempAddress = SearchOffsets(offset::Signatures::LocalPlayerPawn, globals::client);
	//	if (TempAddress == 0)
	//		return false;

	//	offset::dwLocalPlayerPawn = TempAddress + 0x118 - globals::client;

	//	//TempAddress = SearchOffsets(offset::Signatures::ForceJump, globals::client);
	//	//if (TempAddress == 0)
	//	//	return false;

	//	//offset::ForceJump = TempAddress + 0x30 - globals::client;
	//	return true;
	//}

	
}

