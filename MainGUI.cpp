#include "MainGUI.h"
#include "wx/wx.h"
#include <algorithm>
#include <memory>
#include <random>
#include <unordered_map>

MainGUI::MainGUI() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxPoint(30, 30), wxSize(800, 600)) {
  // Create the menu bar
  wxMenuBar *menuBar = new wxMenuBar();
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
  const int buttonIndex = event.GetId() - 10000;
  const int buttonX = buttonIndex % fieldWidth;
  const int buttonY = buttonIndex / fieldWidth;
  DiscoverMine(buttonX, buttonY, buttonIndex);
  event.Skip();
}

void MainGUI::DiscoverMine(int buttonX, int buttonY, int buttonIndex) {
  // Immediatelly return if the button was flagged by the player
  if (buttons.at(buttonIndex)->GetLabel() == L"🚩")
    return;

  // Check if the button was a bomb
  if (fieldMines.at(buttonIndex) == Mine::Planted) {
    DisplayBombsLocation();
    wxMessageBox("CHERNOBYL WAS AVOIDABLE, THIS WAS NOT", "You lost the game");
    GameOverReset();
    return;
  }

  // Field is generated on first click to guarantee no first click loss
  if (clickedSquares == 0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, fieldWidth * fieldHeight - 1);

    for (int minesPlanted = 0; minesPlanted < mines;) {
      const int mineIndex = dis(gen);

      if (fieldMines.at(mineIndex) == Mine::Empty && mineIndex != buttonIndex) {
        fieldMines.at(mineIndex) = Mine::Planted;
        ++minesPlanted;
      }
    }
  }

  Sweep(buttonX, buttonY, buttonIndex);
  if (clickedSquares == (fieldWidth * fieldHeight) - mines) {
    DisplayBombsLocation();
    wxMessageBox("CHERNOBYL WAS AVOIDABLE, CONGRATULATIONS", "You won the game");
    GameOverReset();
  }
}

void MainGUI::Sweep(int buttonX, int buttonY, int buttonIndex) {
  buttons.at(buttonIndex)->Enable(false);
  buttons.at(buttonIndex)->SetLabel("");
  ++clickedSquares;

  const int neighbourMines = CountNeighbours(buttonX, buttonY);
  if (neighbourMines > 0)
    return buttons.at(buttonIndex)->SetLabel(std::to_string(neighbourMines));

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      const bool validIndex = std::clamp(buttonX + i, 0, fieldWidth - 1) == buttonX + i &&
                              std::clamp(buttonY + j, 0, fieldHeight - 1) == buttonY + j;
      if (!validIndex)
        continue;

      const int neighbourIndex = (buttonY + j) * fieldWidth + (buttonX + i);
      if (buttons.at(neighbourIndex)->IsEnabled())
        Sweep(buttonX + i, buttonY + j, neighbourIndex);
    }
  }
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
  const static std::unordered_map<wxString, wxString> labelMap = {
      {"", L"🚩"}, {L"🚩", L"❓"}, {L"❓", ""}};
  const int buttonIndex = event.GetId() - 10000;
  const auto currentLabel = buttons.at(buttonIndex)->GetLabel();
  const auto nextLabel = labelMap.at(currentLabel);
  buttons.at(buttonIndex)->SetLabel(nextLabel);
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
    buttons.at(index)->Bind(wxEVT_RIGHT_UP, &MainGUI::OnButtonRightClicked, this);
    buttonGrid->Add(buttons.at(index).get(), 1, wxEXPAND | wxALL);
    fieldMines.at(index) = Mine::Empty;
  });

  buttonGrid->Layout();
}

void MainGUI::SetDifficulty(wxCommandEvent &event) {
  const static std::unordered_map<int, std::tuple<int, int, int>> difficulties = {
      {ID_EASY, std::make_tuple(8, 8, 10)},
      {ID_MEDIUM, std::make_tuple(16, 16, 40)},
      {ID_HARD, std::make_tuple(30, 16, 99)}};

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
