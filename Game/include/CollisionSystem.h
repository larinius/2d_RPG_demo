#pragma once
#include "precompiled.h"
#include "Object.h"


namespace rpg {

class CollisionBox;

class CollisionSystem : public Object {

public:
  CollisionSystem(std::string name, int id);
  ~CollisionSystem(){onDestroy();}

  // Interface methods
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

private:
  //interface methods
  void onActivate() override;
  void onDestroy() override;

public:
  void addBody(rpg::CollisionBox * body);
  void removeBody(rpg::CollisionBox * body);
  bool isCollide(const rpg::CollisionBox & bodyA, const rpg::CollisionBox & bodyB);
  std::vector<rpg::CollisionBox*> getCollisions(rpg::CollisionBox * capsule);

private:
  std::deque<rpg::CollisionBox*> bodies_;

};
}
