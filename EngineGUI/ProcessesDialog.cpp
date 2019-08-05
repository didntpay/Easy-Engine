#include "ProcessesDialog.h"


wxBEGIN_EVENT_TABLE(ProcessesDialog, wxFrame)
	EVT_BUTTON(1, onConfirmClicked)
	EVT_BUTTON(2, onCancelClicked)
wxEND_EVENT_TABLE()



/*ProcessesDialog::ProcessesDialog(ProcessesDialog& other) // copy constructor 
{		
	SetBackgroundColour(wxColour(232, 232, 232, 255));
	this->confirm = other.confirm;
	this->cancel = other.cancel;
	this->process_list = other.process_list;
	this->processesids = NULL;
	//this->parentpanel = nullptr;
	//printProcessses();
	
}*/

/*ProcessesDialog::ProcessesDialog(ScannerPanel& parent) // constructor with a ScannerPanel
{
	SetBackgroundColour(wxColour(232, 232, 232, 255));
	confirm = new wxButton(this, 1, "Confirm", wxPoint(20, 220), wxDefaultSize);
	cancel = new wxButton(this, 2, "Cancel", wxPoint(120, 220), wxDefaultSize);
	this->process_list = new wxListBox(this, wxID_ANY, wxPoint(6, 10), wxSize(220, 200));
	this->processesids = NULL;
	printProcessses();
	//this->parentpanel = parent;
}*/

/*ProcessesDialog& ProcessesDialog::operator=(const ProcessesDialog& other) 
{
	this->confirm = other.confirm;
	this->cancel = other.cancel;
	this->process_list = other.process_list;
	this->processesids = other.processesids;
	//this->parentpanel = other.parentpanel;
	return *this;
}*/

void ProcessesDialog::printProcessses() // fix the issue of printing random ass name
{
	DWORD processesid[1024];
	DWORD read;
	vector<string> tracker;
	EnumProcesses(processesid, sizeof(processesid), &read);
	int size = sizeof(processesid) / sizeof(DWORD);
	this->processesids = new DWORD[size];
	int lastvalid = -1;
	for (int i = 0; i < size; i++) 
	{
		char processname[1024];
		DWORD rights = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
		HANDLE proc = OpenProcess(rights, false, processesid[i]);
		if (proc == NULL) 
		{
			auto error = GetLastError();
		}
		else 
		{
			GetModuleBaseNameA(proc, NULL, &processname[0], sizeof(processname) / sizeof(char));
			string tmp = processname;
			int found = tmp.find(".exe");

			if(found != string::npos && (lastvalid == -1 || tmp != tracker[lastvalid]))//keep in mind that all module with 
			{//same name would occur together, so comparing with the last one would
			//get to the result
				this->process_list->AppendString(processname);
				tracker.push_back(tmp);
				this->processesids[lastvalid] = processesid[i];
				lastvalid++;
			}
				
		}
		
	}
}


void ProcessesDialog::onConfirmClicked(wxCommandEvent& evt)
{
	int index = process_list->GetSelection();
	
	if(index != wxNOT_FOUND)
		//this->parentpanel->updateProcID(this->processesids[index]);

	evt.Skip();
}

void ProcessesDialog::onCancelClicked(wxCommandEvent& evt)
{
	this->Close();
	evt.Skip();
	
}

ProcessesDialog::ProcessesDialog() :wxFrame(NULL, 8888, "Select Process", wxPoint(30, 30), wxSize(250, 300))
{
	//initlizing the panel when the constructor is called
	//move on with printing processes.
	SetBackgroundColour(wxColour(232, 232, 232, 255));
	confirm = new wxButton(this, 1, "Confirm", wxPoint(20, 220), wxDefaultSize);
	cancel = new wxButton(this, 2, "Cancel", wxPoint(120, 220), wxDefaultSize);
	this->process_list = new wxListBox(this, wxID_ANY, wxPoint(6, 10), wxSize(220, 200));
	this->processesids = NULL;
	//this->parentpanel = NULL;
	printProcessses();
}

ProcessesDialog::~ProcessesDialog()
{
	delete[] this->processesids;
	//delete this->parentpanel;
	//this->parentpanel = NULL;
	this->processesids = NULL;
}