#pragma once
#include "wx/wx.h"

// Possible states of the mines in the field
enum class Mine { Planted, Empty };

class MainGUI : public wxFrame
{
public:
	MainGUI();
	~MainGUI();

protected:
	// Width and height for the minesweeper grid
	int fieldWidth = 10;
	int fieldHeight = 10;

	// Numbers of mines to be displaced on the field
	int mines = 30;

	// Track how many squares were clicked so we can display the winning screen
	int clickedSquares = 0;

	// 1D Array of pointers to buttons on the grid, and the grid sizer
	wxButton** buttons;
	wxGridSizer* buttonGrid;

	// Array of mines to store the state of each of the mines
	Mine* fieldMines = nullptr;

	// Remeber if this is the first click so there's no fail in the first click
	bool isFirstClick = true;

	// Handles the click of any button in the minesweeper grid
	void OnButtonClicked(wxCommandEvent& event);

	// Declare the event table so it can handle the button click
	wxDECLARE_EVENT_TABLE();

	// Functions used in the game over situation
	void DisplayBombsLocation();
	void GameOverReset();

	// Used to generate a new field (on game start, and difficulty change)
	void GenerateNewField(int newFieldWidth, int newFieldHeight, int newMines);

	// The top menu bar
	wxMenuBar* menuBar = nullptr;

	// Difficulty event handlers
	void EasyDifficulty(wxCommandEvent& event);
	void MediumDifficulty(wxCommandEvent& event);
	void HardDifficulty(wxCommandEvent& event);

	// Game menu event handlers
	void NewGame(wxCommandEvent& event);
	void CloseGame(wxCommandEvent& event);
};
