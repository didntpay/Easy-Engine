#pragma once
#ifndef PROCESSDIALOG_H
#define PROCESSDIALOG_H

#include "wx\wx.h"
#include <string>
#include <Windows.h>
#include <psapi.h>
#include <vector>
using namespace std;

//this file is included in ScannerPanel.h
class ScannerPanel; //forward declaration to avoid nested include loops
//see https://stackoverflow.com/questions/4757565/what-are-forward-declarations-in-c for reference

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

