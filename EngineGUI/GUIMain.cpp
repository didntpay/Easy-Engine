#include "GUIMain.h"
wxIMPLEMENT_APP(GUIMain);

GUIMain::GUIMain() 
{

}

GUIMain::~GUIMain() 
{

}

bool GUIMain::OnInit() 
{
	SP = new ScannerPanel();
	SP->Show();
	return true;
}