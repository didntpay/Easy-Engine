// Easy Engin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryScanner.h"
#include <iostream>

int main()
{
	MemoryScanner<float> * ms = new MemoryScanner<float>(10132);
	ms->init();
	ScannerInput SCIN(0, 0);
	ms->firstScan(99.5);
	ms->scanNext(100);
	ms->~MemoryScanner();
	delete ms;
	ms = NULL;

	while (1) {}
}

