#include "stdafx.h"
#include "MemoryScanner.h"
#include <iostream>
MemoryScanner::MemoryScanner(DWORD procID)
{
	this->pid = procID;
}

void MemoryScanner::init()
{
	DWORD access = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION;
	this->hProc = OpenProcess(access, false, this->pid);
}

DWORD MemoryScanner::firstScan()
{
	SYSTEM_INFO info = { 0 };
	GetSystemInfo(&info);

	auto add_sta = (DWORD)info.lpMinimumApplicationAddress;
	auto add_end = (DWORD)info.lpMaximumApplicationAddress;
	
	return 0x10;
}

MemoryScanner::~MemoryScanner()
{
}