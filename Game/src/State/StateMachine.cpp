#include "State/StateMachine.h"
#include "State/State.h"
#include "Globals.h"
#include "Macroses.h"
#include "Engine.h"

std::deque<std::unique_ptr<rpg::State>> rpg::StateMachine::state_queue_;
bool rpg::StateMachine::quit_ = false;

rpg::StateMachine::StateMachine(std::string name, int id) : Object(name, id) {

}

void rpg::StateMachine::update(sf::Time dt) {
  if (!state_queue_.empty()) {
    for (const auto &state : state_queue_) {
      state->update(dt);
    }
  } else {
    quit_ = true;
  }
}

void rpg::StateMachine::draw(sf::RenderWindow *window) {
  for (const auto &state : state_queue_) {
    state->draw(window);
  }
}

void rpg::StateMachine::handleEvent(const sf::Event &event) {
  for (const auto &state : state_queue_) {
    state->handleEvent(event);
  }
}

void rpg::StateMachine::onActivate() {
  state_queue_.emplace_back(std::make_unique<StateTitle>(GameState::Menu));
  state_queue_.front()->setActive(true);
  printf("%s\n", __FUNCTION_NAME__);
}

rpg::State *rpg::StateMachine::get(GameState state) {
  if (state_queue_.empty()) {
    return nullptr;
  }

  auto result = std::find_if(state_queue_.begin(), state_queue_.end(),
                             [&](std::unique_ptr<State> &i) { return i->state() == state; });
  if (result != state_queue_.end()) {
    return result->get();
  } else {
    return nullptr;
  }

}

void rpg::StateMachine::switchState(rpg::GameState state) {
  switch (state) {
  case GameState::Menu: {
    state_queue_.emplace_back(std::make_unique<StateTitle>(GameState::Menu));
    while (state_queue_.size() > 1) {
      state_queue_.front().reset();
      state_queue_.pop_front();
    }
    get(state)->setActive(true);
    break;
  }
  case GameState::Play: {
    state_queue_.emplace_back(std::make_unique<StatePlay>(GameState::Play));
    while (state_queue_.size() > 1) {
      state_queue_.front().reset();
      state_queue_.pop_front();
    }
    get(state)->setActive(true);
    break;
  }

  default:
    break;

    printf("%s\n", __FUNCTION_NAME__);
  }
}

void rpg::StateMachine::quit() {
  quit_ = true;
}
