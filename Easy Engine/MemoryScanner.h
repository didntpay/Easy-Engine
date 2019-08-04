#pragma once
#ifndef MEMORYSCANNER_H
#define MEMORYSCANNER_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include "sorting.h"
using namespace std;

//scan type takes the higher half;
#define ST_INT 0x40
#define ST_FLOAT 0x80
#define ST_BOOL 0x10

//scan method only takes the lower half
#define SME_UNKNOWN 0x01
#define SME_CHANGED 0x02
#define SME_DECREASED 0x05
#define SME_INCREASED 0x09
#define SME_CHANGED_EXACT 0x0E

enum ScanType
{
	int_value = 0x40,
	float_value = 0x08,
	boolean_value = 0x11
};

enum ScanMethod : byte
{
	search_unknown = 0x01,
	search_changed = 0x10,
	search_decreased = 0x80,
	search_increased = 0x90,
	search_changed_exact = 0xCE
};

typedef const struct ScannerInput 
{
	ScanType ST;
	ScanMethod SME;

	ScannerInput(int scanType, int scanMethod = NULL) 
	{
		this->ST = (ScanType)scanType;
		this->SME = (ScanMethod)scanMethod;
	}
} SI;


template <typename T>
class MemoryScanner
{

public:
	typedef struct ScannerOutput
	{
		DWORD  address;
		T  value;
		bool changed;
		T difference;

		ScannerOutput(DWORD add, T val)
		{
			this->address = add;
			this->value = val;
			this->changed = false;
			this->difference = NULL;
		}

		ScannerOutput()
		{
			this->address = NULL;
			this->value = NULL;
			this->changed = false;
			this->difference = NULL;
		}

		bool operator > (const ScannerOutput& other) const
		{
			T difference = this->value - other.value;
			return difference > 0;
		}

		bool operator < (const ScannerOutput& other) const
		{
			T difference = this->value - other.value;
			return difference < 0;
		}
	} SO, NEXT_SCAN_INPUT;

private:
	DWORD pid;
	HANDLE hProc;
	ScannerOutput* SCOU;
	UINT32 size;


public:	

	MemoryScanner<T>::MemoryScanner(DWORD procID)
	{
		this->pid = procID;
		this->hProc = NULL;
		this->SCOU = NULL;
		this->size = NULL;
	}

	//template <typename T>
	void MemoryScanner<T>::init()
	{
		DWORD access = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION;
		this->hProc = OpenProcess(access, false, this->pid);
	}

	//template <typename T>
	DWORD MemoryScanner<T>::firstScan(ScannerInput SCIN, T val)
	{
		SYSTEM_INFO info = { 0 };
		GetSystemInfo(&info);

		DWORD add_sta = (DWORD)info.lpMinimumApplicationAddress;
		DWORD add_end = (DWORD)info.lpMaximumApplicationAddress;
		DWORD pagesize = info.dwPageSize;


		vector<MEMORY_BASIC_INFORMATION> search_region;
		vector<ScannerOutput> SCOU_V; //temporary

		for (DWORD current_base = add_sta; current_base <= add_end;)
		{
			MEMORY_BASIC_INFORMATION mbi = { 0 };
			if (!VirtualQueryEx(this->hProc, (LPVOID)current_base, &mbi, sizeof(mbi)))
			{
				//if return 0, means it failed
				current_base += pagesize;
			}
			else
			{

				if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_GUARD)
				{
					search_region.push_back(mbi);
				}
				current_base += mbi.RegionSize;
			}
		}

		for each (MEMORY_BASIC_INFORMATION mbi in search_region)
		{
			T* buffer = (T*)malloc(mbi.RegionSize + 1);
			if (ReadProcessMemory(this->hProc, mbi.BaseAddress, &buffer[0], mbi.RegionSize, NULL))
			{
				uint8_t increament = sizeof(T);

				for (int i = 0; i < mbi.RegionSize; i += increament)
				{
					DWORD address = (DWORD)mbi.BaseAddress + i;
					T value = buffer[i / sizeof(T)];

					if (value == val)
						SCOU_V.push_back(ScannerOutput(address, value));
				}

			}
			else
			{
				auto error = GetLastError();
				cout << "Read process memory failed, " + error << endl;
			}



			delete[] buffer;
			buffer = NULL;
		}

		this->SCOU = (ScannerOutput*)malloc(sizeof(ScannerOutput) * SCOU_V.size());
		this->size = SCOU_V.size();

		transferElement(SCOU, SCOU_V);
		return 0x10;
	}

	DWORD MemoryScanner<T>::scanNext(T val)
	{
		//DWORD scantype = scanFlag & 0xF0; //take out the lower half
		//DWORD scanmethod = scanFlag & 0x0F;
		uint8_t increment = sizeof(T);
		uint32_t updated_size = 0;
		ScannerOutput* updated_SCOU = (ScannerOutput*)malloc(sizeof(ScannerOutput) * this->size);

		for (int i = 0; i < this->size; i++)
		{
			T* buffer = (T*)malloc(sizeof(T));
			updateScannedList(this->SCOU[i], &buffer[0], increment);
			T updated_value = *buffer;
	

			if (updated_value == val)
			{
				updated_SCOU[i] = ScannerOutput(this->SCOU[i].address, updated_value);
				updated_size++;

				if (updated_value != this->SCOU[i].value)
				{
					updated_SCOU[i].changed = true;
					updated_SCOU[i].difference =  updated_value - this->SCOU[i].value;
				}
				
			}

			delete[] buffer;
			buffer = NULL;
		}

		free(this->SCOU);
		this->SCOU = updated_SCOU;
		this->size = updated_size;
		updated_SCOU = NULL;

		return 0x10;
	}


	void MemoryScanner<T>::updateScannedList(ScannerOutput& SCOU, T* buffer, uint8_t bytes)
	{
		SIZE_T read;
		if (ReadProcessMemory(this->hProc, (LPVOID)SCOU.address, &buffer[0], bytes, &read) == 0)
		{
			auto error = GetLastError();
			cout << "Read process memory failed, " + error << endl;
		}


	}


	void MemoryScanner<T>::transferElement(MemoryScanner<T>::ScannerOutput* values, vector<MemoryScanner<T>::ScannerOutput>& temp)
	{
		int size = temp.size();
		for (int i = 0; i < size; i++)
		{
			values[i] = temp[i];
		}
	}

	MemoryScanner<T>::~MemoryScanner()
	{
		free(this->SCOU);
		this->SCOU = NULL;
	}
};


#endif