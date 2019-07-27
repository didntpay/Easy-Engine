#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
#include "sorting.h"
using namespace std;

enum ScanType 
{
	int_value,
	float_value,
	boolean_value
};

enum ScanMethod
{
	search_unknown,
	search_changed,
	search_decreased,
	search_increased,
	search_changed_exact
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

typedef const struct ScannerOuput 
{
	DWORD  address;
	DWORD  value;

	ScannerOuput(DWORD add, DWORD val)
	{
		this->address = add;
		this->value = val;
	}

	ScannerOuput()
	{
		this->address = NULL;
		this->value = NULL;
	}
} SO;

void transferElement(ScannerOuput* values, vector<ScannerOuput> temp) 
{
	for (int i = 0; i < temp.size; i++) 
	{
		values[i] = temp[i];
	}
}

class MemoryScanner
{
private:
	DWORD pid;
	HANDLE hProc;
	ScannerOuput* SCOU;
	UINT32 size;

public:
	MemoryScanner(DWORD procID);

	void init();

	//template <typename T>
	DWORD MemoryScanner::firstScan(ScannerInput SCIN, int val);

	~MemoryScanner();
};

