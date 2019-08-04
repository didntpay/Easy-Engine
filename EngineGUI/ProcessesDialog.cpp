#include "ProcessesDialog.h"
#include <Windows.h>
#include <psapi.h>
ProcessesDialog::ProcessesDialog(): wxFrame(NULL, wxID_ANY, "Select Process", wxPoint(30, 30), wxSize(250, 300))
{
	SetBackgroundColour(wxColour(232, 232, 232, 255));
	confirm = new wxButton(this, wxID_ANY, "Confirm", wxPoint(20, 220), wxDefaultSize);
	cancel = new wxButton(this, wxID_ANY, "Cancel", wxPoint(120, 220), wxDefaultSize);
	this->process_list = new wxListBox(this, wxID_ANY, wxPoint(6, 10), wxSize(220, 200));
	this->processesid = NULL;
	printProcessses();
}

int ProcessesDialog::getProcessList() 
{
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
	if (!getProcessList())
		return;

	int size = sizeof(processesid) / sizeof(DWORD);
	for (int i = 0; i < size; i++) 
	{
		char processname[1024];
		DWORD rights = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
		HANDLE proc = OpenProcess(rights, false, processesid[i]);
		if (proc == NULL) 
		{
			auto error = GetLastError();
			process_list->Append(wxString((char*)error));
		}
		GetModuleBaseNameA(proc, NULL, &processname[0], sizeof(processname));
		this->process_list->AppendString(processname);
	}
}

ProcessesDialog::~ProcessesDialog() 
{
	delete[] processesid;
	processesid = NULL;
}