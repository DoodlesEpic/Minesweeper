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
  std::unique_ptr<Mine[]> fieldMines;

  wxDECLARE_EVENT_TABLE();

  void OnButtonClicked(wxCommandEvent &event);
  void OnButtonRightClicked(wxMouseEvent &event);

  void GenerateNewField(int newFieldWidth, int newFieldHeight, int newMines);
  void DisplayBombsLocation();
  void GameOverReset();

  void EasyDifficulty(wxCommandEvent &event);
  void MediumDifficulty(wxCommandEvent &event);
  void HardDifficulty(wxCommandEvent &event);
  void NewGame(wxCommandEvent &event);
  void CloseGame(wxCommandEvent &event);
};
