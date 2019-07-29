// Easy Engin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryScanner.h"
#include <iostream>

int main()
{
	MemoryScanner<float> * ms = new MemoryScanner<float>(13084);
	ms->init();
	ScannerInput SCIN(0, 0);
	ms->firstScan(SCIN, 99.5);
	ms->scanNext(ST_INT, 100);
	ms->~MemoryScanner();
	delete ms;
	ms = NULL;

	while (1) {}
}

