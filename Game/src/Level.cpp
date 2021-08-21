#include "precompiled.h"
#include "ECSystem.h"
#include "Engine.h"
#include "Level.h"
#include "Components/AnimatedSprite.h"
//#include "Components/SkeletalAnimation.h"


class Pool;

rpg::Level::Level(const std::string name, const int id) : Object(name, id) {}

void rpg::Level::draw(sf::RenderWindow *window) {
//  IPROF_FUNC;
  tile_map_->draw(window);
}

void rpg::Level::update(sf::Time dt) {

}

void rpg::Level::handleEvent(const sf::Event &event) {

}

void rpg::Level::remove_entity(int id) {
  entities_ = helper::vec_remove(entities_, id);
}

void rpg::Level::onActivate() {
  active_ = true;

  std::string path = "F:/Dev/Src/RPG/Assets/maps/Demo_level.ldtk";
//  std::string path = "F:/Dev/Src/RPG/Assets/maps/Typical_TopDown_example.ldtk";

  load_tilemap(path);

//  spawnAnimatedActor("Dragon_1", 500.f, 500.f);
//  MGR->getEntity("DragonBoy")->setScale(0.1, 0.1);

}

void rpg::Level::load_tilemap(std::string path) {

  int map_id = ID;
  tile_map_ = std::make_unique<rpg::TileMap>(map_id, "map", id_, path);
  tile_map_->setActive(true);
  NAVSYS->setActive(true);
}

int rpg::Level::spawnActor(std::string name, float posX, float posY) {

  int id = ID;

  auto entity = MGR->createEntity(id, name);
  auto ent = MGR->getEntity(entity);
  ent->setOrigin(0.f, 0.f);
  ent->setScale(1.f, 1.f);
  ent->setPosition(posX, posY);

  auto pointer = MGR->addComponent(entity, CType::animatedSprite, name);
//  dynamic_cast<rpg::AnimatedSprite *>(MGR->getComponent(pointer))->loadAnimation("Assets/Sprites/path_icon.json");
  dynamic_cast<rpg::AnimatedSprite *>(MGR->getComponent(pointer))->setPath("Assets/Sprites/Pointer/");

  ent->setActive(true);
  return id;
}

int rpg::Level::spawnAnimatedActor(std::string name, float posX, float posY) {

  auto entity = MGR->createEntity(id_, name);
  auto ent = MGR->getEntity(entity);
  ent->setOrigin(0.f, 0.f);
  ent->setScale(1.f, 1.f);
  ent->setPosition(posX, posY);

//  auto animation = MGR->addComponent(entity, CType::boneAnimation, name);
//  dynamic_cast<rpg::SkeletalAnimation *>(MGR->getComponent(animation))->loadAnimation(name);

  return 0;
}
