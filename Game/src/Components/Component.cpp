#include "precompiled.h"
#include "Components/Component.h"
#include "Helpers.h"
#include "Globals.h"
#include "Engine.h"
#include "Macroses.h"

rpg::Component::Component(const int id, const int owner, const std::string name) :
    id_(id), ownerId_(owner), name_(name), window_(RWIND) {
}

void rpg::Component::draw(sf::RenderWindow *window) {
//  IPROF_FUNC;
}

void rpg::Component::update(sf::Time dt) {
}

void rpg::Component::handleEvent(const sf::Event &event) {

}

void rpg::Component::onActivate() {

  owner_ = MGR->getEntity(ownerId_);
  active_ = true;
  window_ = RWIND;
}

void rpg::Component::onDestroy() {

}

void rpg::Component::setActive(bool active) {

  if (active) {
    onActivate();
    active_ = true;
  }
  else {
    active_ = false;
  }
}

void rpg::Component::removeChild(int id) {
  attachments_ = rpg::helper::vec_remove(attachments_, id);
}
