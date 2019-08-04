#include "ScannerPanel.h"

wxBEGIN_EVENT_TABLE(ScannerPanel, wxFrame)
	EVT_BUTTON(10000, onOpenProcessClicked)
	EVT_BUTTON(10001, onFirstScanCliced)
wxEND_EVENT_TABLE()

ScannerPanel::ScannerPanel() :wxFrame(NULL, wxID_ANY, "Easy Engine Scanner", wxPoint(30,30), wxSize(800,600))
{
	SetBackgroundColour(wxColour(232, 232, 232, 255));
	openprocess = new wxButton(this, 10000, "Open process", wxPoint(25, 20), wxSize(80, 30));
	listctrl = new wxDataViewListCtrl(this, wxID_ANY, wxPoint(20, 80), wxSize(350, 450));
	firstscan = new wxButton(this, 10001, "First scan", wxPoint(500, 80), wxSize(80, 25));
	nextscan = new wxButton(this, wxID_ANY, "Next scan", wxPoint(650, 80), wxSize(80, 25));
	dialog = NULL;
	wxString choices[] = { "Hex", "Float","String" };
	valuetype = new wxComboBox(this, wxID_ANY, "Int32", wxPoint(600, 120), wxSize(80, 30), 2, choices, wxCB_DROPDOWN, wxDefaultValidator, "Value options");


	listctrl->AppendTextColumn("Value");
	listctrl->AppendTextColumn("Address");
	listctrl->AppendTextColumn("Previous value");
	//data.clear();
	listctrl->SetClientSize(wxRect(wxPoint(50, 50), wxPoint(350, 450)));
}

ScannerPanel::~ScannerPanel() 
{
}

void ScannerPanel::onFirstScanCliced(wxCommandEvent& evt) 
{
	/*switch (valuetype->GetSelection())
	{
	
	}*/

	evt.Skip();
}

void ScannerPanel::onOpenProcessClicked (wxCommandEvent& evt)
{
	if (dialog != NULL) 
	{
		evt.Skip();
		return;
	}

	dialog = new ProcessesDialog();
	dialog->Show();
	evt.Skip();
}