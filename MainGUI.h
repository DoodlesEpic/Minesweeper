#pragma once
#include "wx/wx.h"
#include <memory>

enum class Mine { Planted, Empty };

class MainGUI : public wxFrame {
public:
  MainGUI();
  ~MainGUI();

protected:
  bool isFirstClick = true;
  int fieldWidth = 10;
  int fieldHeight = 10;
  int mines = 30;
  int clickedSquares = 0;

  std::vector<std::unique_ptr<wxButton>> buttons;
  wxGridSizer *buttonGrid;
  wxMenuBar *menuBar;
  std::vector<Mine> fieldMines;

  int CountNeighbours(int buttonX, int buttonY);
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
