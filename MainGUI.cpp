#include "MainGUI.h"
#include <algorithm>
#include <memory>
#include <random>
#include <unordered_map>

MainGUI::MainGUI() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(30, 30), wxSize(800, 600)) {
  // Create the menu bar
  menuBar = new wxMenuBar();
  this->SetMenuBar(menuBar);

  // Create the menu bar items for game
  wxMenu *gameMenu = new wxMenu();
  gameMenu->Append(ID_NEW_GAME, "Restart");
  gameMenu->Append(ID_CLOSE_GAME, "Exit");
  menuBar->Append(gameMenu, "Game");

  // Create the menu bar items for difficulty
  wxMenu *difficultyMenu = new wxMenu();
  difficultyMenu->Append(ID_EASY, "Easy");
  difficultyMenu->Append(ID_MEDIUM, "Medium");
  difficultyMenu->Append(ID_HARD, "Hard");
  menuBar->Append(difficultyMenu, "Difficulty");

  // Bind the event handlers to the menu items
  menuBar->Bind(wxEVT_MENU, &MainGUI::NewGame, this, ID_NEW_GAME);
  menuBar->Bind(wxEVT_MENU, &MainGUI::CloseGame, this, ID_CLOSE_GAME);
  menuBar->Bind(wxEVT_MENU, &MainGUI::SetDifficulty, this, ID_EASY);
  menuBar->Bind(wxEVT_MENU, &MainGUI::SetDifficulty, this, ID_MEDIUM);
  menuBar->Bind(wxEVT_MENU, &MainGUI::SetDifficulty, this, ID_HARD);

  // Create a sizer grid to displace the buttons on
  buttonGrid = new wxGridSizer(fieldWidth, fieldHeight, 0, 0);
  this->SetSizer(buttonGrid);

  // Generate a new field to start the game
  GenerateNewField(fieldWidth, fieldHeight, mines);
}

// Handles the click of any button in the minesweeper grid
void MainGUI::OnButtonClicked(wxCommandEvent &event) {
  const int buttonX = (event.GetId() - 10000) % fieldWidth;
  const int buttonY = (event.GetId() - 10000) / fieldHeight;
  const int buttonIndex = buttonY * fieldWidth + buttonX;
  buttons.at(buttonIndex)->Enable(false);
  ++clickedSquares;

  // Check if the button was a bomb
  if (fieldMines.at(buttonIndex) == Mine::Planted) {
    DisplayBombsLocation();
    wxMessageBox("CHERNOBYL WAS AVOIDABLE, THIS WAS NOT", "You lost the game");
    GameOverReset();
    return;
  }

  // Field is generated on first click to guarantee no first click loss
  if (clickedSquares == 1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(0, fieldWidth - 1);
    std::uniform_int_distribution<> disY(0, fieldHeight - 1);

    for (int minesPlanted = 0; minesPlanted < mines;) {
      const int mineX = disX(gen);
      const int mineY = disY(gen);
      const int mineIndex = mineY * fieldWidth + mineX;

      if (fieldMines.at(mineIndex) == Mine::Empty && (mineX != buttonX || mineY != buttonY)) {
        fieldMines.at(mineIndex) = Mine::Planted;
        ++minesPlanted;
      }
    }
  }

  const int neighbourMines = CountNeighbours(buttonX, buttonY);
  if (neighbourMines > 0) {
    buttons.at(buttonIndex)->SetLabel(std::to_string(neighbourMines));
  }

  if (clickedSquares == (fieldWidth * fieldHeight) - mines) {
    DisplayBombsLocation();
    wxMessageBox("CHERNOBYL WAS AVOIDABLE, CONGRATULATIONS", "You won the game");
    GameOverReset();
  }

  event.Skip();
}

int MainGUI::CountNeighbours(int buttonX, int buttonY) {
  int neighbourMines = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      const bool validIndex = std::clamp(buttonX + i, 0, fieldWidth - 1) == buttonX + i &&
                              std::clamp(buttonY + j, 0, fieldHeight - 1) == buttonY + j;
      if (!validIndex)
        continue;

      const int neighbourMineIndex = (buttonY + j) * fieldWidth + (buttonX + i);
      if (fieldMines.at(neighbourMineIndex) == Mine::Planted)
        ++neighbourMines;
    }
  }

  return neighbourMines;
}

void MainGUI::OnButtonRightClicked(wxMouseEvent &event) {
  const int buttonX = (event.GetId() - 10000) % fieldWidth;
  const int buttonY = (event.GetId() - 10000) / fieldHeight;
  const int buttonIndex = buttonY * fieldWidth + buttonX;

  buttons.at(buttonIndex)->GetLabel() == "" ? buttons.at(buttonIndex)->SetLabel(L"🚩")
                                            : buttons.at(buttonIndex)->SetLabel("");
}

void MainGUI::DisplayBombsLocation() {
  std::for_each(fieldMines.begin(), fieldMines.end(), [this](const auto &mine) {
    if (mine == Mine::Planted)
      buttons.at(&mine - &fieldMines.front())->SetLabel(L"💣");
  });
}

void MainGUI::GameOverReset() {
  clickedSquares = 0;
  std::for_each(fieldMines.begin(), fieldMines.end(), [this](const auto &mine) {
    const int index = &mine - &fieldMines.front();
    fieldMines.at(index) = Mine::Empty;
    buttons.at(index)->SetLabel("");
    buttons.at(index)->Enable(true);
  });
}

void MainGUI::GenerateNewField(int newFieldWidth, int newFieldHeight, int newMines) {
  fieldWidth = newFieldWidth;
  fieldHeight = newFieldHeight;
  mines = newMines;

  buttons = std::vector<std::unique_ptr<wxButton>>(fieldWidth * fieldHeight);
  fieldMines = std::vector<Mine>(fieldWidth * fieldHeight);

  buttonGrid->Clear(true);
  buttonGrid->SetCols(fieldWidth);
  buttonGrid->SetRows(fieldHeight);

  std::for_each(fieldMines.begin(), fieldMines.end(), [this](const auto &mine) {
    const int index = &mine - &fieldMines.front();
    wxFont font(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    buttons.at(index) = std::make_unique<wxButton>(this, 10000 + index);
    buttons.at(index)->SetFont(font);
    buttons.at(index)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainGUI::OnButtonClicked, this);
    buttons.at(index)->Bind(wxEVT_RIGHT_DOWN, &MainGUI::OnButtonRightClicked, this);
    buttonGrid->Add(buttons.at(index).get(), 1, wxEXPAND | wxALL);
    fieldMines.at(index) = Mine::Empty;
  });

  buttonGrid->Layout();
}

void MainGUI::SetDifficulty(wxCommandEvent &event) {
  const std::unordered_map<int, std::tuple<int, int, int>> difficulties = {
      {ID_EASY, std::make_tuple(8, 8, 10)},
      {ID_MEDIUM, std::make_tuple(16, 16, 40)},
      {ID_HARD, std::make_tuple(16, 30, 99)}};

  const auto lookupResult = difficulties.find(event.GetId());
  std::tie(fieldWidth, fieldHeight, mines) = lookupResult->second;
  GameOverReset();
  GenerateNewField(fieldWidth, fieldHeight, mines);
}

void MainGUI::NewGame(wxCommandEvent &event) {
  GameOverReset();
  event.Skip();
}

void MainGUI::CloseGame(wxCommandEvent &event) {
  Close();
  event.Skip();
}
