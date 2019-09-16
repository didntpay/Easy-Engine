#pragma once
#ifndef SCANNERPANEL_H
#define SCANNERPANEL_H

#include "wx\wx.h"
#include "wx\dataview.h"
#include "wx\listctrl.h"
#include "..\Easy Engine\MemoryScanner.h"
#include <sstream>

class ProcessesDialog;

#include "ProcessesDialog.h"

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
	LPVOID ms;//memory scanner
	wxTextCtrl* targetvalue;
	int scantype;

	wxMenuBar* meaubar;
	wxMenu* tool_menu;

public:

	ScannerPanel();
	ScannerPanel(ScannerPanel& parent);
	~ScannerPanel();
	
	void onFirstScanCliced(wxCommandEvent& evt);
	void onOpenProcessClicked(wxCommandEvent& evt);

	ScannerPanel& operator= (const ScannerPanel& other);
	void updateProcID(int id);
	void printScannedResult(int type, MemoryScanner<int>* ms_int = NULL, 
							MemoryScanner<float>* ms_float = NULL);
	void onSecondScanClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

private:
	//helper functions
	string toUpperCase(string& original);
};

#endif
