#include "precompiled.h"
#include "StateStack.h"
#include "Globals.h"

namespace rpg {
StateStack::StateStack(Engine * engine)
    : stack(), pendingList(), engine_(engine), factories() {
}

void StateStack::update(sf::Time dt) {
  for (auto itr = stack.rbegin(); itr != stack.rend(); ++itr) {
    if (!(*itr)->update(dt))
      break;
  }

  applyPendingChanges();
}

void StateStack::draw() {
//  IPROF_FUNC;
  for (State::Ptr &state : stack) {
    state->draw();
    if (state->Active()) {
      state->draw();
    }
  }
}

void StateStack::handleEvent(const sf::Event &event) {
  for (auto itr = stack.rbegin(); itr != stack.rend(); ++itr) {
    if (!(*itr)->handleEvent(event))
      break;
  }

  applyPendingChanges();
}

void StateStack::pushState(StateID stateID) {
  pendingList.emplace_back(PendingChange(Push, stateID));
}

void StateStack::popState() {
  pendingList.emplace_back(PendingChange(Pop));
}

void StateStack::clearStates() {
  pendingList.emplace_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const {
  return stack.empty();
}

State::Ptr StateStack::createState(StateID stateID) {
  auto found = factories.find(stateID);
  assert(found != factories.end());

  return found->second();
}

void StateStack::applyPendingChanges() {
  for (PendingChange change : pendingList) {

    switch (change.action) {

    case Push:
      stack.push_back(createState(change.stateID));
      if (!stack.empty())
        stack.back()->onActivate();
      break;

    case Pop:
      stack.back()->onDestroy();
      stack.pop_back();

      if (!stack.empty())
        stack.back()->onActivate();
      break;

    case Clear:
      for (State::Ptr &state : stack)
        state->onDestroy();

      stack.clear();
      break;
    }
  }

  pendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, StateID stateID)
    : action(action), stateID(stateID) {
}

}
