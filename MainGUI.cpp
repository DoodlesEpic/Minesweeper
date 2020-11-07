#include "MainGUI.h"

// The event table maps events to handler functions
// The ids here and where we build the components must be equal
wxBEGIN_EVENT_TABLE(MainGUI, wxFrame)
wxEND_EVENT_TABLE()

// Init a frame without a parent frame
// Init with any frame ID
// Also set the title of the frame
// Make the default starting location 30x30
// Make the default size 800x600
MainGUI::MainGUI() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(30, 30), wxSize(800, 600))
{
	// Create the whole grid of buttons
	buttons = new wxButton * [fieldWidth * fieldHeight];

	// Create a sizer grid to displace the buttons on
	wxGridSizer* buttonGrid = new wxGridSizer(fieldWidth, fieldHeight, 0, 0);

	// Dinamically generate the minesweep field
	for (int i = 0; i < fieldWidth; i++)
	{
		for (int j = 0; j < fieldHeight; j++)
		{
			// Get the button index from the 2d grid to the 1d array
			const int buttonIndex = j * fieldWidth + i;

			// Create a new button and bind an event handler to the clicked event
			buttons[buttonIndex] = new wxButton(this, 10000 + buttonIndex);
			buttons[buttonIndex]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainGUI::OnButtonClicked, this);

			// Put the new button inside the button grid
			buttonGrid->Add(buttons[buttonIndex], 1, wxEXPAND | wxALL);
		}
	}

	// Use the sizer and update the layout
	this->SetSizer(buttonGrid);
	buttonGrid->Layout();
}

MainGUI::~MainGUI()
{
	// Delete the buttons array to prevent memleak
	// We don't need to handle the buttons themselves because wx does it for us
	delete[] buttons;
}

// Handles the click of any button in the minesweeper grid
void MainGUI::OnButtonClicked(wxCommandEvent& event)
{
	event.Skip();
}
