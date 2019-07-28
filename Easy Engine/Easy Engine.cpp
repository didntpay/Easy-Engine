// Easy Engin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryScanner.h"
#include <iostream>

int main()
{
	MemoryScanner * ms = new MemoryScanner(22192);
	ms->init();
	ScannerInput SCIN(0, 0);
	ms->firstScan(SCIN, 100);
	ms->~MemoryScanner();
	delete ms;
	ms = NULL;

	while (1) {}
}

