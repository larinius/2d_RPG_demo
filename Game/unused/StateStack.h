#pragma once
#include "precompiled.h"
#include "State/State.h"
//#include "Engine.h"

namespace rpg {

//class Context;
class Engine;
enum class StateID;

class StateStack : private sf::NonCopyable {
public:
  enum Action {
    Push,
    Pop,
    Clear,
  };

public:
  explicit StateStack(Engine *engine);

  template<typename T>
  void registerState(StateID stateID);

  template<typename T, typename Param1>
  void registerState(StateID stateID, Param1 arg1);

  void update(sf::Time dt);

  void draw();

  void handleEvent(const sf::Event &event);

  void pushState(StateID stateID);

  void popState();

  void clearStates();

  bool isEmpty() const;

//  Context& get_context(){return context;}

private:
  rpg::State::Ptr createState(StateID stateID);

  void applyPendingChanges();

private:
  struct PendingChange {
    explicit PendingChange(Action action, StateID stateId = StateID::None);

    Action action;
    StateID stateID;
  };

private:
  std::vector<State::Ptr> stack;
  std::vector<PendingChange> pendingList;
//  Context context;
  rpg::Engine *engine_;
  std::map<StateID, std::function<State::Ptr()>> factories;
};

template<typename T>
void StateStack::registerState(StateID stateID) {
  factories[stateID] = [this]() {
//    return State::Ptr(new T(*this, engine_));
    return State::Ptr(new T(*this));
  };
}

template<typename T, typename Param1>
void StateStack::registerState(StateID stateID, Param1 arg1) {
  factories[stateID] = [this, arg1]() {
//    return State::Ptr(new T(*this, engine_, arg1));
    return State::Ptr(new T(*this, arg1));
  };
}

}
