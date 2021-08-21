#include "precompiled.h"
#include "CollisionBox.h"
#include "CollisionSystem.h"
#include "Macroses.h"
#include "SelbaWard.hpp"
#include "Entity.h"
#include "Engine.h"

rpg::CollisionBox::CollisionBox(int id, int owner, std::string name) : Component(id, owner, name) {
}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::CollisionBox::draw(sf::RenderWindow *window) {
  Component::draw(window);
  if (drawDebug_) {
    /*
     * Debug Draw
     */
    sf::CircleShape circle;
    circle.setRadius(32.f);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(1);
    circle.setPosition(position_.x, position_.y);
    window->draw(circle);
  }
}

void rpg::CollisionBox::update(sf::Time dt) {
  Component::update(dt);
  position_ = mathfu::Vector<float, 2>(owner_->getPosition().x - size_ * 0.5f,
                                       owner_->getPosition().y - size_ * 0.5f);
//  auto bodies = COLLSYS->getBodies();
//  for(const auto & c: bodies){
//    if(COLLSYS->isCollide(*this, *c)){
//      printf("Collide: %s\n", c->name().c_str());
//    }
//  }

//  auto coll = COLLSYS->getCollisions(this);
//  if(!coll.empty()){
//    printf("Collide!\n");
//  }
//  int i = 0;
}

void rpg::CollisionBox::handleEvent(const sf::Event &event) {
  Component::handleEvent(event);
}

void rpg::CollisionBox::setActive(bool active) {
  Component::setActive(active);
}

void rpg::CollisionBox::onActivate() {
  Component::onActivate();
  registerSelf();
}

void rpg::CollisionBox::onDestroy() {
  Component::onDestroy();
}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

void rpg::CollisionBox::onCollide() {

}

void rpg::CollisionBox::registerSelf() {
  COLLSYS->addBody(this);
}

std::vector<rpg::CollisionBox *> rpg::CollisionBox::getCollisions() {
  return std::vector<rpg::CollisionBox *>();
}
