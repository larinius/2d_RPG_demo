#pragma once
#include "precompiled.h"
#include "Component.h"
#include "CollisionSystem.h"

namespace rpg {

class CollisionBox : public Component {
public:
  CollisionBox(int id, int owner, std::string name);

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;
  void setActive(bool active) override;

private:
  void onActivate() override;
  void onDestroy() override;

private:
  void onCollide();
  void registerSelf();
  std::vector<CollisionBox*> getCollisions();

private:
//  mathfu::Vector<float, 2> capsule_pos_{0.0f, 0.0f};
//  int size_{static_cast<int>(GRID)};

  bool drawDebug_{false};


};

}
