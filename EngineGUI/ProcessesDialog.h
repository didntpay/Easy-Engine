#pragma once
#ifndef PROCESS_DIALOG_H

#define PROCESS_DIALOG_H

#include "wx\wx.h"
#include <string>
#include <Windows.h>
#include <psapi.h>
#include <vector>
using namespace std;

class ScannerPanel;

#include "ScannerPanel.h"

class ProcessesDialog : public wxFrame //the panel to pop up when open process is clicked
{
private:
	DWORD* processesids;
	wxListBox* process_list;
	wxButton* confirm;
	wxButton* cancel;
	ScannerPanel* parentpanel;


public:
	ProcessesDialog();
	//ProcessesDialog(ProcessesDialog& other);
	ProcessesDialog(ScannerPanel& parent);
	//ProcessesDialog& operator=(const ProcessesDialog& other);
	~ProcessesDialog();
	void printProcessses();
	void onConfirmClicked(wxCommandEvent& evt);
	void onCancelClicked(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};
#endif

