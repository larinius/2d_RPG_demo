#include "ActorState.h"
#include "ECSystem.h"
#include "Engine.h"
#include "AnimatedSprite.h"

rpg::ActorState::ActorState(int id, int owner, std::string name) : Component(id, owner, name) {

}

void rpg::ActorState::draw(sf::RenderWindow *window) {
  Component::draw(window);
}

void rpg::ActorState::update(sf::Time dt) {
  Component::update(dt);
}

void rpg::ActorState::handleEvent(const sf::Event &event) {
  Component::handleEvent(event);
}

void rpg::ActorState::onActivate() {
  Component::onActivate();
}

void rpg::ActorState::onDestroy() {
  Component::onDestroy();
}

void rpg::ActorState::setBehavior(rpg::Behavior behavior) {
  behavior_ = behavior;
}

void rpg::ActorState::setAction(rpg::Action action) {
  if (action_ != action) {
    action_ = action;
  }
}

void rpg::ActorState::setDirection(rpg::Direction direction) {
  direction_ = direction;
}
