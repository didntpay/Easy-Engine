#pragma once
#include "wx\wx.h"
#include <string>
#include <Windows.h>
#include <psapi.h>
#include <vector>
using namespace std;
class ProcessesDialog : public wxFrame//the panel to pop up when open process is clicked
{
private:
	DWORD* processesid;
	wxListBox* process_list;
	wxButton* confirm;
	wxButton* cancel;


public:
	ProcessesDialog();
	~ProcessesDialog();
	void printProcessses();
};

