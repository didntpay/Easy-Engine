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

string MemoryScanner::firstScan()
{
	SYSTEM_INFO info = { 0 };
	GetSystemInfo(&info);

	long add_sta = (long)info.lpMinimumApplicationAddress;
	long add_end = (long)info.lpMaximumApplicationAddress;

	return add_sta + " " + add_end;
}

MemoryScanner::~MemoryScanner()
{
}