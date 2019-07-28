#pragma once

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
private:
	DWORD pid;
	HANDLE hProc;
	ScannerOutput* SCOU;
	UINT32 size;


public:

	typedef struct ScannerOutput
	{
		DWORD  address;
		T  value;
		bool changed;
		DWORD difference;

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
			int difference = this->value - other.value;
			return difference > 0;
		}

		bool operator < (const ScannerOutput& other) const
		{
			int difference = this->value - other.value;
			return difference < 0;
		}
	} SO, NEXT_SCAN_INPUT;

	MemoryScanner(DWORD procID);

	void init();

	//template <typename T>
	DWORD firstScan(ScannerInput SCIN, int val);


	DWORD scanNext(DWORD scanFlag, T val);
	void updateScannedList(ScannerOutput& SCOU, unsigned char* buffer, DWORD scanflag);
	~MemoryScanner();
};

#include "MemoryScanner.cpp"