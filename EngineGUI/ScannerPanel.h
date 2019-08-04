#pragma once
#include "wx\wx.h"
#include "wx\dataview.h"
#include "ProcessesDialog.h"
#include "..\Easy Engine\MemoryScanner.h"
class ScannerPanel :public wxFrame
{
private:
	wxButton* firstscan;
	wxButton* nextscan;
	wxButton* openprocess;
	wxDataViewListCtrl* listctrl;
	wxComboBox* valuetype;
	ProcessesDialog* dialog;
	DWORD procID;

public:
	ScannerPanel();
	ScannerPanel(const ScannerPanel& other);
	~ScannerPanel();
	
	void onFirstScanCliced(wxCommandEvent& evt);
	void onOpenProcessClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

