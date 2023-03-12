#pragma once
#include "MainGUI.h"
#include "wx/wx.h"

class App : public wxApp {
public:
  App();
  ~App();
  virtual bool OnInit();

private:
  MainGUI *mainWindow = nullptr;
};
