#include "App.h"
wxIMPLEMENT_APP(App);

App::App() {}

App::~App() {}

bool App::OnInit() {
  // Create and show the main GUI on application init
  mainWindow = new MainGUI();
  mainWindow->Show();

  return true;
}