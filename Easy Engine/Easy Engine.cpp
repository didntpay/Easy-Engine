// Easy Engin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MemoryScanner.h"
#include <iostream>

int main()
{
	MemoryScanner<float> * ms = new MemoryScanner<float>(8228);
	ms->init();
	ScannerInput SCIN(0, 0);
	ms->firstScan(100);
	//ms->scanNext(100);
	cout << "Address           Values" << endl;
	for (int i = 0; i < ms->getSize(); i++)
	{
		cout << ms->getSCOU()[i].address;
		cout << "   " << ms->getSCOU()[i].value << endl;
	}
	ms->~MemoryScanner();
	delete ms;
	ms = NULL;

	while (1) {}
}

