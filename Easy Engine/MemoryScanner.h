#pragma once

#include <Windows.h>
#include <string>
using namespace std;
class MemoryScanner
{
private:
	DWORD pid;
	HANDLE hProc;
public:
	MemoryScanner(DWORD procID);

	void init();

	DWORD firstScan();

	~MemoryScanner();
};

