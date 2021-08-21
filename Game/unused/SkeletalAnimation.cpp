#include "precompiled.h"
#include "Components/SkeletalAnimation.h"
#include <SFMLFactory.h>
#include <SFMLArmatureDisplay.h>
#include "Globals.h"
#include "Engine.h"
#include "Macroses.h"

rpg::SkeletalAnimation::SkeletalAnimation(int id, int owner, std::string name)
    : Component(id, owner, name) {
  window_ = RWIND;
}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::SkeletalAnimation::draw(sf::RenderWindow *window) {
  Component::draw(window);
  window_->draw(*armature);
}

void rpg::SkeletalAnimation::update(sf::Time dt) {
  Component::update(dt);
  float deltaTime = clock.restart().asSeconds();
  factory->update(deltaTime);

}

void rpg::SkeletalAnimation::onActivate() {
  Component::onActivate();
}

void rpg::SkeletalAnimation::onDestroy() {
  Component::onDestroy();
}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

void rpg::SkeletalAnimation::loadTexture(std::string name) {

}

void rpg::SkeletalAnimation::loadAnimation(std::string name) {
  texture = std::make_unique<sf::Texture>();
  texture->loadFromFile("Assets/Animations/"+name+"_tex.png");
  texture->setSmooth(true);

  factory = std::make_unique<dragonBones::SFMLFactory>();



  factory->loadDragonBonesData("Assets/Animations/"+name+"_ske.json");
  factory->loadTextureAtlasData("Assets/Animations/"+name+"_tex.json", texture.get());

  armature = std::make_unique<dragonBones::SFMLArmatureDisplay>("Dragon");

//  auto armatureDisplay = new dragonBones::SFMLArmatureDisplay("Dragon");
  armature->getAnimation()->play("walk");
  armature->setPosition({ 512.f, 440.f });

}
