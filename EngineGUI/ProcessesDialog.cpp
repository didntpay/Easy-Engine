#include "ProcessesDialog.h"

ProcessesDialog::ProcessesDialog(): wxFrame(NULL, wxID_ANY, "Select Process", wxPoint(30, 30), wxSize(250, 300))
{
	//initlizing the panel when the constructor is called
	//move on with printing processes.
	SetBackgroundColour(wxColour(232, 232, 232, 255));
	confirm = new wxButton(this, wxID_ANY, "Confirm", wxPoint(20, 220), wxDefaultSize);
	cancel = new wxButton(this, wxID_ANY, "Cancel", wxPoint(120, 220), wxDefaultSize);
	this->process_list = new wxListBox(this, wxID_ANY, wxPoint(6, 10), wxSize(220, 200));
	this->processesid = NULL;
	printProcessses();
}

void ProcessesDialog::printProcessses()
{
	DWORD processesid[1024];
	DWORD read;
	EnumProcesses(processesid, sizeof(processesid), &read);
	int size = sizeof(processesid) / sizeof(DWORD);
	vector<string> tracker;

	for (int i = 0; i < size; i++) 
	{
		char processname[1024];
		DWORD rights = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
		HANDLE proc = OpenProcess(rights, false, processesid[i]);
		if (proc == NULL) 
		{
			auto error = GetLastError();
			tracker.push_back("");
		}
		else 
		{
			GetModuleBaseNameA(proc, NULL, &processname[0], sizeof(processname) / sizeof(char));
			string tmp = processname;
			int found = tmp.find(".exe");

			if(found != string::npos && tmp != tracker[i - 1])//keep in mind that all module with 
			{//same name would occur together, so comparing with the last one would
			//get to the result
				this->process_list->AppendString(processname);
				tracker.push_back(tmp);
			}
			else 
			{
				tracker.push_back("");
			}
				
		}
		
	}
}

ProcessesDialog::~ProcessesDialog() 
{
	delete[] processesid;
	processesid = NULL;
}