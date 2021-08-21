#pragma once
#include "precompiled.h"
#include "Component.h"
#include "Physics.h"

namespace rpg {

class PhysicsBox : public rpg::Component, sf::Transformable {
public:
  explicit PhysicsBox(int id, int owner, std::string name, rpg::Body &body);
  PhysicsBox(const PhysicsBox &) = delete;
  PhysicsBox &operator=(const PhysicsBox &) = delete;

  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

  // Event API
  void onActivate() override;
  void onDestroy() override;

  void setVelocity(float x, float y, float force);

private:
  int b2d_id_;
  rpg::Body* body_;

  friend class PlayerController;
  friend class ActorController;
  friend class Engine;
  friend class Physics;
};

}
