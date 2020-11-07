#pragma once
#include "wx/wx.h"
#include "MainGUI.h"

class App : public wxApp
{
public:
	App();
	~App();
	virtual bool OnInit();

private:
	MainGUI* mainWindow = nullptr;
};
