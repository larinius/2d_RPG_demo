#include "precompiled.h"
#include "SimpleImage.h"
#include "Engine.h"

rpg::SimpleImage::SimpleImage(int id, int owner, std::string name) : Component(id, owner, name) {

}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::SimpleImage::draw(sf::RenderWindow *window) {
  window_->draw(sprite);
//  window_->draw(image_);
}

void rpg::SimpleImage::update(sf::Time dt) {
  Component::update(dt);

  auto pos = owner_->getPosition();
  position_ = {pos.x, pos.y};

  this->setPosition(pos);
  sprite.setPosition(pos);
//  image_.setPosition(owner_->getPosition());
}

void rpg::SimpleImage::onActivate() {

  rpg::Component::onActivate();
  sprite.setPosition(owner_->getPosition());
  sprite.setOrigin(owner_->getOrigin());
  sprite.setRotation(owner_->getRotation());
  sprite.setScale(owner_->getScale());

//  image_.setRadius(50.f);
//  image_.setPosition(owner_->getPosition());
//  image_.setFillColor(sf::Color::Cyan);
  set_image(path_);
}

void rpg::SimpleImage::onDestroy() {

}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

void rpg::SimpleImage::set_image(std::string path) {
  path_ = path;
//  texture.loadFromFile("Assets/Sprites/Player_front.png");
  texture.loadFromFile(path_);
  texture.setSmooth(true);
  sprite.setTexture(texture);

}
