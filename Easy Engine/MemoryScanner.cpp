#include "stdafx.h"
#include "MemoryScanner.h"

MemoryScanner::MemoryScanner(DWORD procID)
{
	this->pid = procID;
}

void MemoryScanner::init()
{
	DWORD access = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION;
	this->hProc = OpenProcess(access, false, this->pid);
}

//template <typename T>
DWORD MemoryScanner::firstScan(ScannerInput SCIN)
{
	SYSTEM_INFO info = { 0 };
	GetSystemInfo(&info);

	DWORD add_sta = (DWORD)info.lpMinimumApplicationAddress;
	DWORD add_end = (DWORD)info.lpMaximumApplicationAddress;
	DWORD pagesize = info.dwPageSize;
	vector<MEMORY_BASIC_INFORMATION> search_region;

	for (DWORD current_base = add_sta; current_base < add_end;)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (!VirtualQueryEx(this->hProc, &current_base, &mbi, sizeof(mbi)))
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
	SIZE_T read;
	vector<ScannerOuput> SCOU;
	for each (MEMORY_BASIC_INFORMATION mbi in search_region) 
	{		
		byte * buffer = new byte[mbi.RegionSize];
		if (ReadProcessMemory(this->hProc, mbi.BaseAddress, buffer, mbi.RegionSize, &read))
		{
			uint8_t increament;
			switch (SCIN.ST) 
			{
				case ScanType::int_value:
					increament = sizeof(int);
					break;
				case ScanType::float_value:
					increament = sizeof(float);
					break;
				case ScanType::boolean_value:
					increament = sizeof(bool);
			}
			for (int i = 0; i < mbi.RegionSize; i += increament) 
			{
				DWORD address = (DWORD)mbi.BaseAddress + i * increament;
				int32_t value = NULL;
				for (int j = 0; j < increament; j++) 
				{
					value = value << 8 + buffer[j];
				}
				SCOU.push_back(ScannerOuput(address, value));
			}

		}
		else 
		{
			auto error = GetLastError();
			cout << "Read process memory failed, " + error << endl;
		}

		delete[] buffer;
	}
	return 0x10;
}

MemoryScanner::~MemoryScanner()
{
}