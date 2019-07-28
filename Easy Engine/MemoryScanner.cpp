#include "stdafx.h"
#include "MemoryScanner.h"

template <typename T>
void transferElement(ScannerOutput* values, vector<ScannerOutput>& temp);

template <typename T>
MemoryScanner<T>::MemoryScanner(DWORD procID)
{
	this->pid = procID;
	this->hProc = NULL;
	this->SCOU = NULL;
	this->size = NULL;
}

template <typename T>
void MemoryScanner<T>::init()
{
	DWORD access = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION;
	this->hProc = OpenProcess(access, false, this->pid);
}

template <typename T>
DWORD MemoryScanner<T>::firstScan(ScannerInput SCIN, int val)
{
	SYSTEM_INFO info = { 0 };
	GetSystemInfo(&info);

	DWORD add_sta = (DWORD)info.lpMinimumApplicationAddress;
	DWORD add_end = (DWORD)info.lpMaximumApplicationAddress;
	DWORD pagesize = info.dwPageSize;

	int found = 0;

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
		unsigned char * buffer = new unsigned char[mbi.RegionSize + 1];
		if (ReadProcessMemory(this->hProc, mbi.BaseAddress, &buffer[0], mbi.RegionSize, NULL))
		{
			int read = 0;
			uint8_t increament = 4;

			for (int i = 0; i < mbi.RegionSize; i += increament) 
			{
				DWORD address = (DWORD)mbi.BaseAddress + i;
				int value = NULL;
				for (int j = increament - 1; j >= 0; j--) 
				{
					value = value << (8 * j) | buffer[j + read];
				}
				read += increament;
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

template<typename T>
DWORD MemoryScanner<T>::scanNext(DWORD scanFlag, T val)
{
	DWORD scantype = scanFlag & 0xF0; //take out the lower half
	DWORD scanmethod = scanFlag & 0x0F;
	uint8_t increment = sizeof(T);
	ScannerOutput* updated_SCOU = (ScannerOutput*)malloc(sizeof(ScannerOutput) * this->size);

	for (int i = 0; i < this->size; i++) 
	{
		unsigned char* buffer = new unsigned char[sizeof(T)];
		updateScannedList(this->SCOU[i], buffer, scantype);
		T updated_value = NULL;
		for (int i = increment - 1; i >= 0; i--)
		{
			updated_value = updated_value << (i * 8) | buffer[i];
		}

		if (updated_value == val && updated_value != this->SCOU[i].value) 
		{
			this->SCOU[i].changed = true;
			this->SCOU[i].difference = this->SCOU[i].value - updated_value;
			this->SCOU[i].value = updated_value;
		}
		else if (updated_value == this->SCOU[i].value) 
		{
			this->SCOU[i].changed = false;
			this->SCOU[i].difference = NULL;
		}

		updated_SCOU[i] = ScannerOutput(this->SCOU[i].address, updated_value);

		delete[] buffer;
		buffer = NULL;
	}

	delete[] this->SCOU;
	this->SCOU = updated_SCOU;
	updated_SCOU = NULL;

	return 0x10;
}

template <typename T>
void MemoryScanner<T>::updateScannedList(ScannerOutput& SCOU, unsigned char* buffer, DWORD scanflag) 
{
	if (ReadProcessMemory(this->hProc, (LPVOID)SCOU.address, &buffer[0], (int)scanflag, NULL) == 0)
	{
		auto error = GetLastError();
		cout << "Read process memory failed, " + error << endl;
	}
		
	
}

template <typename T>
void transferElement(MemoryScanner<T>::ScannerOutput* values, vector<MemoryScanner<T>::ScannerOutput>& temp)
{
	int size = temp.size();
	for (int i = 0; i < size; i++)
	{
		values[i] = temp[i];
	}
}

template<typename T>
MemoryScanner<T>::~MemoryScanner()
{
	free(this->SCOU);
	this->SCOU = NULL;
}