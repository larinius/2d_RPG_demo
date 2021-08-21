#pragma once
#include "precompiled.h"
#include "Object.h"
#include "State.h"

namespace rpg {

class StateMachine : public Object {
public:
  StateMachine(std::string name, int id);

  void update(sf::Time dt) override;
  void draw(sf::RenderWindow *window) override;
  void handleEvent(const sf::Event &event) override;

  static State *get(GameState state);
  static void switchState(GameState state);
  bool isQuit(){return quit_;}
  static void quit();

private:
  void onActivate() override;
  void onDestroy() override {};

private:
  static std::deque<std::unique_ptr<State>> state_queue_;
  static bool quit_;

};
}
