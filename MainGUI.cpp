#include "MainGUI.h"

// Init a frame without a parent frame
// Init with any frame ID
// Also set the title of the frame
// Make the default starting location 30x30
// Make the default size 800x600
MainGUI::MainGUI() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(30, 30), wxSize(800, 600))
{
}

MainGUI::~MainGUI()
{
}
