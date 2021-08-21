#pragma once
#include "precompiled.h"
#include "State.h"

namespace rpg {

class StateTitle : public State {
public:
  StateTitle(GameState state);
  ~StateTitle() override;

  void update(sf::Time dt) override;
  void draw(sf::RenderWindow *window) override;
  void handleEvent(const sf::Event &event) override;


private:
  void onActivate() override;
  void onDestroy() override;

};

}
