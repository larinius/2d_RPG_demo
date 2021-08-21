#include "precompiled.h"
#include "Navigation.h"

rpg::Navigation::Navigation(int id, int owner, std::string name) : Component(id, owner, name) {

}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::Navigation::draw(sf::RenderWindow *window) {
  Component::draw(window);
}

void rpg::Navigation::update(sf::Time dt) {
  Component::update(dt);
}

void rpg::Navigation::onActivate() {
  Component::onActivate();
}

void rpg::Navigation::onDestroy() {
  Component::onDestroy();
}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////
