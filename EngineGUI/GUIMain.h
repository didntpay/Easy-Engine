#pragma once
#ifndef GUIMAIN_H
#define GUIMAIN_H

#include "wx\wx.h"
#include "ScannerPanel.h"

class GUIMain:public wxApp
{
private:
	ScannerPanel* SP;

public:

	GUIMain();
	~GUIMain();
	virtual bool OnInit();
	

};

#endif

