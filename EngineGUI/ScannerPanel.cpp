#include "ScannerPanel.h"


wxBEGIN_EVENT_TABLE(ScannerPanel, wxFrame)
	EVT_BUTTON(10000, onOpenProcessClicked)
	EVT_BUTTON(10001, onFirstScanCliced)
	EVT_BUTTON(10002, onSecondScanClicked)
wxEND_EVENT_TABLE()


ScannerPanel::ScannerPanel() :wxFrame(NULL, wxID_ANY, "Easy Engine Scanner", wxPoint(30,30), wxSize(800,600))
{
	SetBackgroundColour(wxColour(232, 232, 232, 255));
	openprocess = new wxButton(this, 10000, "Open process", wxPoint(25, 20), wxSize(80, 30));
	listctrl = new wxDataViewListCtrl(this, wxID_ANY, wxPoint(20, 80), wxSize(350, 450));
	firstscan = new wxButton(this, 10001, "First scan", wxPoint(500, 80), wxSize(80, 25));
	nextscan = new wxButton(this, 10002, "Next scan", wxPoint(650, 80), wxSize(80, 25));
	//dialog = NULL;
	wxString choices[] = { "Int32", "Hex", "Float" };
	valuetype = new wxComboBox(this, wxID_ANY, "Int32", wxPoint(600, 120), wxSize(80, 30), 3, choices, wxCB_DROPDOWN, wxDefaultValidator, "Value options");
	targetvalue = new wxTextCtrl(this, wxID_ANY, "enter a value here", wxPoint(400, 220), wxSize(300, 20),
		NULL);
	scantype = NULL;

	meaubar = new wxMenuBar();
	tool_menu = new wxMenu();
	tool_menu->Append(wxID_OPEN, "AI finder");
	meaubar->Append(tool_menu, "Tools");
	SetMenuBar(meaubar);

	listctrl->AppendTextColumn("Value");
	listctrl->AppendTextColumn("Address");
	listctrl->AppendTextColumn("Previous value");

	listctrl->SetClientSize(wxRect(wxPoint(50, 50), wxPoint(350, 450)));
}

ScannerPanel::ScannerPanel(ScannerPanel& parent)
{
	this->procID = parent.procID; // for now, only thing needed is procID
}

ScannerPanel::~ScannerPanel() 
{
}

string ScannerPanel::toUpperCase(string& original)
{
	string convereted;
	for (int i = 0; i < original.size(); i++)
	{
		char currentchar = original.at(i);
		if (currentchar > 96 && currentchar < 123)
		{
			//ASCII difference
			currentchar -= 32;
		}
		convereted += currentchar;
	}
	return convereted;
}

void ScannerPanel::printScannedResult(int type, MemoryScanner<int>* ms_int, 
									 MemoryScanner<float>* ms_float)
{
	if (((ms_int != NULL && ms_int->getSize() == 0) || (ms_float != NULL && ms_float->getSize() == 0)))
	{
		this->listctrl->ClearColumns();
		this->listctrl->AppendTextColumn("Value");
		this->listctrl->AppendTextColumn("Address");
		this->listctrl->AppendTextColumn("Previous value");
		return;
	}

	//this->listctrl->ClearColumns();
	if (type == 0)
	{
		const MemoryScanner<int>::ScannerOutput* result = ms_int->getSCOU();
		

		for (int i = 0; i < ms_int->getSize(); i++)
		{
			MemoryScanner<int>::ScannerOutput entry = result[i];
			wxVector<wxVariant> data;
			stringstream ss;
			//converting int to hex
			ss << std::hex << entry.address;

			data.push_back(toUpperCase(ss.str()));
			data.push_back(std::to_string(entry.value));
			if (entry.changed)
			{
				data.push_back(std::to_string(entry.difference));
			}
			else
			{
				data.push_back(NULL);
			}

			this->listctrl->AppendItem(data);
			data.clear();
		}
	}
	else if (type == 1)
	{
		//hex
	}
	else 
	{
		//float
		const MemoryScanner<float>::ScannerOutput* result = ms_float->getSCOU();

		for (int i = 0; i < ms_float->getSize(); i++)
		{
			MemoryScanner<float>::ScannerOutput entry = result[i];
			wxVector<wxVariant> data;
			stringstream ss;
			//converting address(int) to hex
			ss << std::hex << entry.address;

			data.push_back(toUpperCase(ss.str()));
			data.push_back(std::to_string(entry.value));
			if (entry.changed)
			{
				data.push_back(std::to_string(entry.difference));
			}
			else
			{
				data.push_back(NULL);
			}

			this->listctrl->AppendItem(data);
			data.clear();
		}
	}
}

void ScannerPanel::onSecondScanClicked(wxCommandEvent& evt)
{
	if (this->ms == NULL)
	{
		return;
	}

	if (this->scantype == 0)
	{
		//int
		int target;
		string searchvalue = this->targetvalue->GetLineText(0);
		stringstream convereter(searchvalue);
		convereter >> target;
		MemoryScanner<int>* scanner_cast = (MemoryScanner<int>*)this->ms;
		
		scanner_cast->scanNext(target);
		
		//ms = &scanner_cast;
		printScannedResult(0, scanner_cast, NULL);
	}
	else if (this->scantype == 1)
	{
		//hex
	}
	else 
	{
		//float
	}
}

void ScannerPanel::onFirstScanCliced(wxCommandEvent& evt) 
{
	if (this->procID == NULL)//print a message box later
		return;
	int temp = this->valuetype->GetCurrentSelection();
	this->scantype = temp;
	string searchvalue = this->targetvalue->GetLineText(0);
	stringstream converter(searchvalue);
	if (temp == 0)
	{
		//int
		this->ms = new MemoryScanner<int>(this->procID);//construct the object first, then give it to ms
		MemoryScanner<int>* scanner_cast = (MemoryScanner<int>*)this->ms;
		scanner_cast->init();
		int target = 0;
		converter >> target;
		scanner_cast->firstScan(target);

		printScannedResult(0, scanner_cast, NULL);
	}
	else if (temp == 1)
	{
		//hex
		this->ms = new MemoryScanner<DWORD>(this->procID);
		MemoryScanner<DWORD> scanner_cast = *((MemoryScanner<DWORD>*)this->ms);
		float target = 0;
		converter >> target;
		scanner_cast.firstScan(target);
		printScannedResult(0);
	}
	else
	{
		//float
		this->ms = new MemoryScanner<float>(this->procID);
		MemoryScanner<float>* scanner_cast = (MemoryScanner<float>*)this->ms;
		float target = 0;
		converter >> target;
		scanner_cast->init();
		scanner_cast->firstScan(target);
		printScannedResult(2, NULL, scanner_cast);
	}
	evt.Skip();
}

void ScannerPanel::onOpenProcessClicked (wxCommandEvent& evt)
{
	if (this->dialog->FindWindowById(8888) != NULL) // id is defined in ProcessesDialog.cpp
	{
		evt.Skip();
		return;
	}

	this->dialog =  new ProcessesDialog(*this);
	this->dialog->Show();
	evt.Skip();
}

ScannerPanel& ScannerPanel:: operator = (const ScannerPanel& other)
{
	this->procID = other.procID;
	return *this;
}

void ScannerPanel::updateProcID(int id) 
{
	this->procID = id;


}

void ProcessesDialog::onConfirmClicked(wxCommandEvent& evt) //this is calling a method of ScannerPanel
{//from ProcessesDialog.cpp but ScannerPanel is only forward declared there
 // so in order to call the function, the function needs to be defined here so it has a definition of ScannePanel
	int index = this->process_list->GetSelection();
	//this referes to ProcessesDialog
	//see https://stackoverflow.com/questions/4757565/what-are-forward-declarations-in-c for reference
	if (index != wxNOT_FOUND)
	{
		this->parentpanel->updateProcID(this->processesids[index - 1]);
		evt.Skip();
		this->Hide();
	}
	else 
	{
		//Print out ID index is not in the list
		evt.Skip();
	}
	
	
}