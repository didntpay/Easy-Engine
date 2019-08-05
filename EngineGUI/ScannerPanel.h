#pragma once
#ifndef SCANNER_PANEL_H
#define SCANNER_PANEL_H

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
	//ProcessesDialog* dialog = nullptr;
	ProcessesDialog* dialog;
	DWORD procID;

public:

	ScannerPanel();
	ScannerPanel(ScannerPanel& parent);
	~ScannerPanel();
	
	void onFirstScanCliced(wxCommandEvent& evt);
	void onOpenProcessClicked(wxCommandEvent& evt);

	ScannerPanel& operator= (const ScannerPanel& other);
	void updateProcID(int id);

	wxDECLARE_EVENT_TABLE();
};

#endif
