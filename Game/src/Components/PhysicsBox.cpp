#include "precompiled.h"
#include "Components/PhysicsBox.h"
#include "Physics.h"
#include "Engine.h"
#include "Globals.h"
#include "Macroses.h"

rpg::PhysicsBox::PhysicsBox(int id, int owner, std::string name, rpg::Body &body) :
    Component(id, owner, name), body_(&body) {
}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::PhysicsBox::draw(sf::RenderWindow *window) {
  IPROF_FUNC;
//  Component::draw();
  //dynamic_cast<Physics *>(rpg::Engine::get("Physics"))->draw(RWIND);
}

void rpg::PhysicsBox::update(sf::Time dt) {
  IPROF_FUNC;
  Component::update(dt);

}

void rpg::PhysicsBox::handleEvent(const sf::Event &event) {
  Component::handleEvent(event);
}

void rpg::PhysicsBox::onActivate() {

//  std::string name = owner_->name() + "_collision_box";
//  float posX = owner_->getPosition().x;
//  float posY = owner_->getPosition().y;
////  b2d_id_ = rpg::Engine::eng().physics().createAABB(name, posX, posY);
}

void rpg::PhysicsBox::onDestroy() {
  Component::onDestroy();
}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

void rpg::PhysicsBox::setVelocity(float x, float y, float force) {
  IPROF_FUNC;

  /**
   * Must stop body before use new force
   */
  body_->body->SetLinearVelocity(b2Vec2(0.f, 0.f));
  body_->body->SetAngularVelocity(0);

  if (force == 0) {
    body_->body->SetLinearDamping(1000.f);
  } else {
    b2Vec2 vel(x, y);
    vel.Normalize();
    vel.x = vel.x * force;
    vel.y = vel.y * force;

    body_->body->SetLinearDamping(100.f);
    body_->body->ApplyForce(vel, body_->body->GetWorldCenter(), true);
  }
}
