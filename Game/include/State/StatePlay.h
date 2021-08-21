#pragma once
#include "precompiled.h"
#include "State.h"

namespace rpg {

class StatePlay : public State {
public:
  StatePlay(GameState state);
  ~StatePlay() override;

  void update(sf::Time dt) override;
  void draw(sf::RenderWindow *window) override;
  void handleEvent(const sf::Event &event) override;

private:
  void onActivate() override;
  void onDestroy() override;

};

}
