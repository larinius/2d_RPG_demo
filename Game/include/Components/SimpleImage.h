#pragma once
#include "precompiled.h"
#include "Component.h"

namespace rpg {

class SimpleImage : public rpg::Component, public sf::Transformable
    {
public:
  SimpleImage(int id, int owner, std::string name);

  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;

  // Event API
  void onActivate() override;
  void onDestroy() override;
//  void setActive(bool active) override;

  void set_image(std::string path);

private:
  sf::CircleShape image_;

  sf::Texture texture;
  sf::Sprite sprite;

  std::string path_ = "Assets/Sprites/blank.png";


};

}
