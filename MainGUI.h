#pragma once
#include "wx/wx.h"

class MainGUI : public wxFrame
{
public:
	MainGUI();
	~MainGUI();

public:
	// Width and height for the minesweeper grid
	int fieldWidth = 10;
	int fieldHeight = 10;

	// 1D Array of pointers to buttons on the grid
	wxButton** buttons;

	// Handles the click of any button in the minesweeper grid
	void OnButtonClicked(wxCommandEvent& event);

	// Declare the event table so it can handle the button click
	wxDECLARE_EVENT_TABLE();
};
