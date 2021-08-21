#pragma once
#include "precompiled.h"

namespace rpg {

enum class GameState {
  Loading,
  Menu,
  Play,
  Pause,
  Settings,
  Quit
};

/**
 * Base class for GameStates. Similar to Object class.
 */
class State {
public:
  State(GameState state) : state_(state) {};
  State(State const &) = delete;
  State &operator=(State const &) = delete;

  virtual ~State() = default;

  // Interface methods
  virtual void draw(sf::RenderWindow *window) = 0;
  virtual void update(sf::Time dt) = 0;
  virtual void handleEvent(const sf::Event &event) = 0;

  void setActive(bool active);

  bool isActive() { return active_; }

  GameState state() { return state_; }

  void popMe() { popMe_ = true; }
  bool isPop() { return popMe_; }

protected:
  //interface methods
  virtual void onActivate() = 0;
  virtual void onDestroy() = 0;

  bool active_{false};
  const GameState state_;
  bool popMe_{false};

};

}
