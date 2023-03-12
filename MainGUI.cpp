#include "MainGUI.h"
#include <memory>

// The event table maps events to handler functions
// The ids here and where we build the components must be equal
wxBEGIN_EVENT_TABLE(MainGUI, wxFrame) EVT_MENU(20001, MainGUI::EasyDifficulty);
EVT_MENU(20002, MainGUI::MediumDifficulty);
EVT_MENU(20003, MainGUI::HardDifficulty);
EVT_MENU(20004, MainGUI::NewGame);
EVT_MENU(20005, MainGUI::CloseGame) wxEND_EVENT_TABLE();

// Init a frame without a parent frame
// Init with any frame ID
// Also set the title of the frame
// Make the default starting location 30x30
// Make the default size 800x600
MainGUI::MainGUI()
    : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(30, 30),
              wxSize(800, 600)) {
  // Create the menu bar
  menuBar = new wxMenuBar();
  this->SetMenuBar(menuBar);

  // Create the menu bar items for game
  wxMenu *gameMenu = new wxMenu();
  gameMenu->Append(20004, "Restart");
  gameMenu->Append(20005, "Exit");
  menuBar->Append(gameMenu, "Game");

  // Create the menu bar items for difficulty
  wxMenu *difficultyMenu = new wxMenu();
  difficultyMenu->Append(20001, "Easy");
  difficultyMenu->Append(20002, "Medium");
  difficultyMenu->Append(20003, "Hard");
  menuBar->Append(difficultyMenu, "Difficulty");

  // Create a sizer grid to displace the buttons on
  buttonGrid = new wxGridSizer(fieldWidth, fieldHeight, 0, 0);
  this->SetSizer(buttonGrid);

  // Generate a new field to start the game
  GenerateNewField(fieldWidth, fieldHeight, mines);
}

MainGUI::~MainGUI() {
  // Delete the buttons and fieldMines array to prevent memleaks
  // We don't need to handle the buttons themselves because wx does it for us
  delete[] buttons;
  delete[] fieldMines;
}

// Handles the click of any button in the minesweeper grid
void MainGUI::OnButtonClicked(wxCommandEvent &event) {
  // Find the coordinates of the clicked button
  const int buttonX = (event.GetId() - 10000) % fieldWidth;
  const int buttonY = (event.GetId() - 10000) / fieldHeight;

  // Get the button index from the 2d grid to the 1d array
  const int buttonIndex = buttonY * fieldWidth + buttonX;

  // Prevent this button from being clicked again
  buttons[buttonIndex]->Enable(false);

  // Track how many squares we have clicked already
  ++clickedSquares;

  // On the first click, generate the field mines
  if (isFirstClick) {
    // For each mine there is on the field
    for (int minesPlanted = 0; minesPlanted < mines;) {
      // Find the coordinates of the current mine
      const int mineX = rand() % fieldWidth;
      const int mineY = rand() % fieldHeight;

      // Get the mine index from the 2d grid to the 1d array
      const int mineIndex = mineY * fieldWidth + mineX;

      // Make sure there is no mine there yet
      // Also make sure the first button will not be a mine
      if (fieldMines[mineIndex] == Mine::Empty && mineX != buttonX &&
          mineY != buttonY) {
        // Plant a mine at the location
        fieldMines[mineIndex] = Mine::Planted;

        // Count the mine planted
        ++minesPlanted;
      }
    }

    // Make sure we don't generate the mine field again
    isFirstClick = false;
  }

  // Check if the button was a bomb
  if (fieldMines[buttonIndex] == Mine::Planted) {
    DisplayBombsLocation();
    wxMessageBox("CHERNOBYL WAS AVOIDABLE, THIS WAS NOT", "You lost the game");
    GameOverReset();
  }

  // There's no mine planted, count the surrounding mines and show the game win
  // screen
  else {
    // Count neighbour mines to display the label
    int neighbourMines = 0;
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        // Guarante this is not an out of bounds index
        const bool isValidIndex = buttonX + i >= 0 &&
                                  buttonX + i < fieldWidth &&
                                  buttonY + j >= 0 && buttonY + j < fieldHeight;

        if (isValidIndex) {
          // Get the mine index from the 2d grid to the 1d array
          const int neighbourMineIndex =
              (buttonY + j) * fieldWidth + (buttonX + i);

          // If there's a bomb planted at the location, count neighbour mines
          if (fieldMines[neighbourMineIndex] == Mine::Planted) {
            ++neighbourMines;
          }
        }
      }
    }

    // If there are any neighbour mines, update the label count
    if (neighbourMines > 0) {
      buttons[buttonIndex]->SetLabel(std::to_string(neighbourMines));
    }

    // Check for the winning game condition
    if (clickedSquares == (fieldWidth * fieldHeight) - mines) {
      DisplayBombsLocation();
      wxMessageBox("CHERNOBYL WAS AVOIDABLE, CONGRATULATIONS",
                   "You won the game");
      GameOverReset();
    }
  }

  event.Skip();
}

void MainGUI::OnButtonRightClicked(wxMouseEvent &event) {
  // Find the coordinates of the clicked button
  const int buttonX = (event.GetId() - 10000) % fieldWidth;
  const int buttonY = (event.GetId() - 10000) / fieldHeight;

  // Get the button index from the 2d grid to the 1d array
  const int buttonIndex = buttonY * fieldWidth + buttonX;

  if (buttons[buttonIndex]->GetLabel() == "") {
    buttons[buttonIndex]->SetLabel(L"🚩");
  } else {
    buttons[buttonIndex]->SetLabel("");
  }
}

// Display where the bombs where planted
void MainGUI::DisplayBombsLocation() {
  for (int i = 0; i < fieldWidth; i++) {
    for (int j = 0; j < fieldHeight; j++) {
      // Get the mine index from the 2d grid to the 1d array
      const int mineIndex = j * fieldWidth + i;

      if (fieldMines[mineIndex] == Mine::Planted) {
        buttons[mineIndex]->SetLabel(L"💣");
      }
    }
  }
}

// Resets the game field and first click variable
void MainGUI::GameOverReset() {
  // Reset the game
  isFirstClick = true;
  clickedSquares = 0;

  // We only clear the field, but don't regenerate it
  // This is because the next click will trigger the field generation
  for (int i = 0; i < fieldWidth; i++) {
    for (int j = 0; j < fieldHeight; j++) {
      // Get the grid index from the 2d grid to the 1d array
      const int index = j * fieldWidth + i;

      // Remove any planted mines
      fieldMines[index] = Mine::Empty;

      // Reset the label and enable the button
      buttons[index]->SetLabel("");
      buttons[index]->Enable(true);
    }
  }
}

void MainGUI::GenerateNewField(int newFieldWidth, int newFieldHeight,
                               int newMines) {
  // Change the difficulty
  fieldWidth = newFieldWidth;
  fieldHeight = newFieldHeight;
  mines = newMines;

  // Guarantee no memory leaks
  delete[] buttons;
  delete[] fieldMines;

  // Remove all buttons and ajust columns and rows
  buttonGrid->Clear(true);
  buttonGrid->SetCols(fieldWidth);
  buttonGrid->SetRows(fieldHeight);

  // Create the new grid of buttons
  buttons = new wxButton *[fieldWidth * fieldHeight];

  // Initialize the array for the mines states too
  fieldMines = new Mine[fieldWidth * fieldHeight];

  // Create a font to use on the buttons
  wxFont font(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,
              false);

  // Dinamically generate the minesweep field
  for (int i = 0; i < fieldWidth; i++) {
    for (int j = 0; j < fieldHeight; j++) {
      // Get the button index from the 2d grid to the 1d array
      const int buttonIndex = j * fieldWidth + i;

      // Create a new button and bind an event handler to the clicked event
      buttons[buttonIndex] = new wxButton(this, 10000 + buttonIndex);
      buttons[buttonIndex]->SetFont(font);
      buttons[buttonIndex]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                                 &MainGUI::OnButtonClicked, this);
      buttons[buttonIndex]->Connect(
          wxEVT_RIGHT_DOWN, wxMouseEventHandler(MainGUI::OnButtonRightClicked),
          NULL, this);

      // Initialize the default mine state to empty
      fieldMines[buttonIndex] = Mine::Empty;

      // Put the new button inside the button grid
      buttonGrid->Add(buttons[buttonIndex], 1, wxEXPAND | wxALL);
    }
  }

  // Use the sizer and update the layout
  buttonGrid->Layout();
}

void MainGUI::EasyDifficulty(wxCommandEvent &event) {
  // Reset the game
  GameOverReset();

  // Generate new field with correct difficulty
  GenerateNewField(5, 5, 6);
}

void MainGUI::MediumDifficulty(wxCommandEvent &event) {
  // Reset the game
  GameOverReset();

  // Generate new field with correct difficulty
  GenerateNewField(10, 10, 30);
}

void MainGUI::HardDifficulty(wxCommandEvent &event) {
  // Reset the game
  GameOverReset();

  // Generate new field with correct difficulty
  GenerateNewField(15, 15, 80);
}

void MainGUI::NewGame(wxCommandEvent &event) {
  GameOverReset();
  event.Skip();
}

void MainGUI::CloseGame(wxCommandEvent &event) {
  Close();
  event.Skip();
}
