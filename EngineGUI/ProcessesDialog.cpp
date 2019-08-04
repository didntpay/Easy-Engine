#include "ProcessesDialog.h"
//#include <tlhelp32.h>
#include <Windows.h>
#include <psapi.h>
ProcessesDialog::ProcessesDialog(): wxFrame(NULL, wxID_ANY, "Select Process", wxPoint(30, 30), wxSize(200, 300))
{
	this->process_list = new wxListBox(this, wxID_ANY);
	this->confirm = new wxButton(this, 0, "Confirm");
	this->cancel = new wxButton(this, 1, "Cancel");
	this->processesid = NULL;
}

int ProcessesDialog::getProcessList() 
{
	/*HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, TH32CS_SNAPALL);
	if (snapshot == INVALID_HANDLE_VALUE) 
	{
		auto error = GetLastError();
		return 0;
	}
	else 
	{
		PROCESSENTRY32 pde;
		Process32First(snapshot, &pde);
		vector<DWORD> tmp;
		tmp.push_back(pde.th32ProcessID);
		while(Process32Next(snapshot, &pde))
	}*/
	processesid = new DWORD[1014];
	DWORD numofproc;
	if (EnumProcesses(processesid, sizeof(processesid), &numofproc)) 
	{
		int size = (int)numofproc / sizeof(DWORD);
		DWORD* tmp = new DWORD[size];
		for (int i = 0; i < size; i++)
		{
			tmp[i] = processesid[i];
		}

		delete[] processesid;
		processesid = tmp;
		delete[] tmp;
		tmp = NULL;
		return 1;
	}
	else 
	{
		return 0; //call getlasterror when call this function
	}
}

void ProcessesDialog::printProcessses()
{

}

ProcessesDialog::~ProcessesDialog() 
{
	delete[] processesid;
	processesid = NULL;
}