#include "stdafx.h"
#include "MemoryScanner.h"

MemoryScanner::MemoryScanner(DWORD procID)
{
	this->pid = procID;
	this->hProc = NULL;
	this->SCOU = NULL;
	this->size = NULL;
}

void MemoryScanner::init()
{
	DWORD access = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION;
	this->hProc = OpenProcess(access, false, this->pid);
}

//template <typename T>
DWORD MemoryScanner::firstScan(ScannerInput SCIN, int val)
{
	SYSTEM_INFO info = { 0 };
	GetSystemInfo(&info);

	DWORD add_sta = (DWORD)info.lpMinimumApplicationAddress;
	DWORD add_end = (DWORD)info.lpMaximumApplicationAddress;
	DWORD pagesize = info.dwPageSize;
	vector<MEMORY_BASIC_INFORMATION> search_region;

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
	int found = 0;
	vector<ScannerOuput> SCOU_V; //temporary
	for each (MEMORY_BASIC_INFORMATION mbi in search_region) 
	{		
		unsigned char * buffer = new unsigned char[mbi.RegionSize + 1];
		if (ReadProcessMemory(this->hProc, mbi.BaseAddress, &buffer[0], mbi.RegionSize, NULL))
		{
			int read = 0;
			uint8_t increament = 4;

			for (int i = 0; i < mbi.RegionSize; i += increament) 
			{
				DWORD address = (DWORD)mbi.BaseAddress + i;
				int32_t value = NULL;
				for (int j = increament - 1; j >= 0; j--) 
				{
					value = value << (8 * j) | buffer[j + read];
				}
				read += increament;
				if (value == val)
					SCOU_V.push_back(ScannerOuput(address, value));

			}

		}
		else 
		{
			auto error = GetLastError();
			cout << "Read process memory failed, " + error << endl;
		}

		this->SCOU = (ScannerOuput*)malloc(sizeof(ScannerOuput) * SCOU_V.size);
		transferElement(this->SCOU, SCOU_V);
		this->size = SCOU_V.size;
		quicksort(0, this->size, this->SCOU);//don't run until < and > is overloaded


		delete[] buffer;
		buffer = NULL;
	}
	return 0x10;
}

MemoryScanner::~MemoryScanner()
{
	free(this->SCOU);
	this->SCOU = NULL;
}