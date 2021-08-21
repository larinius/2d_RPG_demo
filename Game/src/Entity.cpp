#include "precompiled.h"
#include "Entity.h"
#include "Engine.h"
#include "ActorState.h"
#include "AnimatedSprite.h"
#include "ActorController.h"

rpg::Entity::Entity(const int id, const std::string name) :
    id_(id), name_(name) {
}

void rpg::Entity::onDestroy() {
}

void rpg::Entity::onActivate() {
  active_ = true;
//  this->setPosition(0.f, 0.f);

//  printf("Active: %s\n", this->name().c_str());
  for (auto c : components_) {
    MGR->getComponent(c)->setActive(true);
  }

}

void rpg::Entity::setActive(bool active) {

  if (active) {
    active_ = true;
    onActivate();
  } else {
    active_ = false;
  }

  for (const auto &c : components_) {
    auto comp = MGR->getComponent(c);
    if (comp != nullptr) {
      comp->setActive(active_);
    }
  }
}

void rpg::Entity::update(sf::Time dt) {

}

void rpg::Entity::draw(sf::RenderWindow *window) {
//  IPROF_FUNC;
}

void rpg::Entity::handleEvent(const sf::Event &event) {

}

void rpg::Entity::removeComponent(int id) {
  components_ = helper::vec_remove(components_, id);
}

void rpg::Entity::removeTag(int id) {
  tags_ = helper::vec_remove(tags_, id);
}

void rpg::Entity::removeChild(int id) {
  attachments_ = helper::vec_remove(attachments_, id);
}

void rpg::Entity::onCommand(rpg::Command command) {

  auto comp = dynamic_cast<rpg::ActorController *>(MGR->getComponent(id_, "CommandComponent"));
  if (comp != nullptr) {
    comp->onCommand(command);
  }

}
