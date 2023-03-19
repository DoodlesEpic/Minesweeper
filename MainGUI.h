#pragma once
#include "wx/wx.h"
#include <memory>

enum class Mine { Planted, Empty };

enum ID {
  ID_EASY = 20001,
  ID_MEDIUM,
  ID_HARD,
  ID_NEW_GAME,
  ID_CLOSE_GAME,
};

class MainGUI : public wxFrame {
public:
  MainGUI();

protected:
  int fieldWidth = 16;
  int fieldHeight = 16;
  int mines = 40;
  int clickedSquares = 0;

  std::vector<std::unique_ptr<wxButton>> buttons;
  wxGridSizer *buttonGrid;
  wxMenuBar *menuBar;
  std::vector<Mine> fieldMines;

  int CountNeighbours(int buttonX, int buttonY);
  void DiscoverMine(int buttonX, int buttonY, int buttonIndex);
  void Sweep(int buttonX, int buttonY, int buttonIndex);
  void GenerateNewField(int newFieldWidth, int newFieldHeight, int newMines);
  void DisplayBombsLocation();
  void GameOverReset();

private:
  void OnButtonClicked(wxCommandEvent &event);
  void OnButtonRightClicked(wxMouseEvent &event);
  void SetDifficulty(wxCommandEvent &event);
  void NewGame(wxCommandEvent &event);
  void CloseGame(wxCommandEvent &event);
};
