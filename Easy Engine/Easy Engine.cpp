// Easy Engin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryScanner.h"
#include <iostream>
int main()
{
	MemoryScanner * ms = new MemoryScanner(18592);
	ms->init();
	cout << ms->firstScan() + "he" << endl;
	delete ms;
	while (1) {}
}

