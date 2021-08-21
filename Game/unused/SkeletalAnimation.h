#pragma once
#include "precompiled.h"
#include <SFMLFactory.h>
#include <SFMLArmatureDisplay.h>
#include "Component.h"

namespace rpg {

class SkeletalAnimation : public rpg::Component, public sf::Transformable
{
public:
  SkeletalAnimation(int id, int owner, std::string name);

  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;

  // Event API
  void onActivate() override;
  void onDestroy() override;
//  void setActive(bool active) override;

  void loadTexture(std::string path);
  void loadAnimation(std::string path);

private:
  std::unique_ptr<sf::Texture> texture;
  std::string path_ = "Assets/Sprites/blank.png";
  std::unique_ptr<dragonBones::SFMLArmatureDisplay> armature;
  std::unique_ptr<dragonBones::SFMLFactory> factory;

  sf::Time frameTime;
  sf::Time currentTime;
  sf::Clock clock;

  int currentFrame;
  bool isLooped{true};
  bool isPaused{false};

};

}
