// Easy Engin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryScanner.h"
#include <iostream>
int main()
{
	MemoryScanner * ms = new MemoryScanner(21312);
	ms->init();
	ScannerInput SCIN(0, 0);
	ms->firstScan(SCIN);
	delete ms;
	while (1) {}
}

