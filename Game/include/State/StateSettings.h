#pragma once
#include "precompiled.h"
#include "State.h"

namespace rpg {

class StateSettings : public State {
public:
  StateSettings(GameState State);
  ~StateSettings()  override ;

  void draw();
  void update(sf::Time dt);
  void handleEvent(const sf::Event &event);
  void onActivate();
  void onDestroy();

private:
  bool isInitialised;

  tgui::GuiSFML gui;
  tgui::Theme theme;

  void initGUI();

};

}
