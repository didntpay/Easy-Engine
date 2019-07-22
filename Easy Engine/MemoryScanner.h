#pragma once

#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>
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
};

typedef const struct ScannerOuput 
{
	DWORD  address;
	DWORD  value;

	ScannerOuput(DWORD add, DWORD val)
	{
		this->address = add;
		this->value = val;
	}
};

class MemoryScanner
{
private:
	DWORD pid;
	HANDLE hProc;

public:
	MemoryScanner(DWORD procID);

	void init();

	//template <typename T>
	DWORD MemoryScanner::firstScan(ScannerInput SCIN);

	~MemoryScanner();
};

