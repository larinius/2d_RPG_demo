#pragma once
#include "precompiled.h"
#include "Component.h"

namespace rpg {

class Navigation : public Component {
public:
  Navigation(int id, int owner, std::string name);

  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;

  // Event API
  void onActivate() override;
  void onDestroy() override;
//  void setActive(bool active) override;

private:
//  std::
};
}
