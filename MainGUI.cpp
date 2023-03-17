#include "MainGUI.h"
#include <memory>

// Disable clang-format because it doesn't know how to ident these macros
// clang-format off
wxBEGIN_EVENT_TABLE(MainGUI, wxFrame) EVT_MENU(20001, MainGUI::EasyDifficulty)
EVT_MENU(20002, MainGUI::MediumDifficulty)
EVT_MENU(20003, MainGUI::HardDifficulty)
EVT_MENU(20004, MainGUI::NewGame)
EVT_MENU(20005, MainGUI::CloseGame) wxEND_EVENT_TABLE()

MainGUI::MainGUI() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(30, 30), wxSize(800, 600)) {
  // clang-format on
  // Reenable clang-format as it's not a problem to format the following code

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

MainGUI::~MainGUI() {}

// Handles the click of any button in the minesweeper grid
void MainGUI::OnButtonClicked(wxCommandEvent &event) {
  const int buttonX = (event.GetId() - 10000) % fieldWidth;
  const int buttonY = (event.GetId() - 10000) / fieldHeight;
  const int buttonIndex = buttonY * fieldWidth + buttonX;
  buttons[buttonIndex]->Enable(false);
  ++clickedSquares;

  if (isFirstClick) {
    for (int minesPlanted = 0; minesPlanted < mines;) {
      const int mineX = rand() % fieldWidth;
      const int mineY = rand() % fieldHeight;
      const int mineIndex = mineY * fieldWidth + mineX;

      if (fieldMines[mineIndex] == Mine::Empty && mineX != buttonX &&
          mineY != buttonY) {
        fieldMines[mineIndex] = Mine::Planted;
        ++minesPlanted;
      }
    }

    isFirstClick = false;
  }

  // Check if the button was a bomb
  if (fieldMines[buttonIndex] == Mine::Planted) {
    DisplayBombsLocation();
    wxMessageBox("CHERNOBYL WAS AVOIDABLE, THIS WAS NOT", "You lost the game");
    GameOverReset();
  }

  else {
    int neighbourMines = 0;
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        const bool isValidIndex = buttonX + i >= 0 &&
                                  buttonX + i < fieldWidth &&
                                  buttonY + j >= 0 && buttonY + j < fieldHeight;

        if (isValidIndex) {
          const int neighbourMineIndex =
              (buttonY + j) * fieldWidth + (buttonX + i);

          if (fieldMines[neighbourMineIndex] == Mine::Planted) {
            ++neighbourMines;
          }
        }
      }
    }

    if (neighbourMines > 0) {
      buttons[buttonIndex]->SetLabel(std::to_string(neighbourMines));
    }

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
  const int buttonX = (event.GetId() - 10000) % fieldWidth;
  const int buttonY = (event.GetId() - 10000) / fieldHeight;
  const int buttonIndex = buttonY * fieldWidth + buttonX;

  if (buttons[buttonIndex]->GetLabel() == "") {
    buttons[buttonIndex]->SetLabel(L"🚩");
  } else {
    buttons[buttonIndex]->SetLabel("");
  }
}

void MainGUI::DisplayBombsLocation() {
  for (int i = 0; i < fieldWidth; i++) {
    for (int j = 0; j < fieldHeight; j++) {
      const int mineIndex = j * fieldWidth + i;

      if (fieldMines[mineIndex] == Mine::Planted) {
        buttons[mineIndex]->SetLabel(L"💣");
      }
    }
  }
}

void MainGUI::GameOverReset() {
  isFirstClick = true;
  clickedSquares = 0;

  for (int i = 0; i < fieldWidth; i++) {
    for (int j = 0; j < fieldHeight; j++) {
      const int index = j * fieldWidth + i;
      fieldMines[index] = Mine::Empty;
      buttons[index]->SetLabel("");
      buttons[index]->Enable(true);
    }
  }
}

void MainGUI::GenerateNewField(int newFieldWidth, int newFieldHeight,
                               int newMines) {
  fieldWidth = newFieldWidth;
  fieldHeight = newFieldHeight;
  mines = newMines;

  buttonGrid->Clear(true);
  buttonGrid->SetCols(fieldWidth);
  buttonGrid->SetRows(fieldHeight);

  buttons =
      std::make_unique<std::shared_ptr<wxButton>[]>(fieldWidth * fieldHeight);
  fieldMines = std::make_unique<Mine[]>(fieldWidth * fieldHeight);

  wxFont font(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,
              false);

  for (int i = 0; i < fieldWidth; i++) {
    for (int j = 0; j < fieldHeight; j++) {
      const int buttonIndex = j * fieldWidth + i;

      buttons[buttonIndex] =
          std::make_shared<wxButton>(this, 10000 + buttonIndex);
      buttons[buttonIndex]->SetFont(font);
      buttons[buttonIndex]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,
                                 &MainGUI::OnButtonClicked, this);
      buttons[buttonIndex]->Connect(
          wxEVT_RIGHT_DOWN, wxMouseEventHandler(MainGUI::OnButtonRightClicked),
          NULL, this);
      buttonGrid->Add(buttons[buttonIndex].get(), 1, wxEXPAND | wxALL);

      fieldMines[buttonIndex] = Mine::Empty;
    }
  }

  buttonGrid->Layout();
}

void MainGUI::EasyDifficulty(wxCommandEvent &event) {
  GameOverReset();
  GenerateNewField(5, 5, 6);
}

void MainGUI::MediumDifficulty(wxCommandEvent &event) {
  GameOverReset();
  GenerateNewField(10, 10, 30);
}

void MainGUI::HardDifficulty(wxCommandEvent &event) {
  GameOverReset();
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
