#include "precompiled.h"
#include "ECSystem.h"
#include <Components/SimpleImage.h>
#include <Components/PlayerController.h>
#include <Components/AnimatedSprite.h>
//#include <Components/SkeletalAnimation.h>
#include <Sound.h>
#include <CollisionBox.h>
#include "Level.h"
#include "Entity.h"
#include "Components/Component.h"
#include "Engine.h"
#include "Components/SimpleImage.h"
#include "Components/PhysicsBox.h"
#include "Components/ActorState.h"
#include "Components/ActorController.h"

rpg::ECSystem::ECSystem(std::string name, int id) : Object(name, id) {}

void rpg::ECSystem::draw(sf::RenderWindow *window) {
//  IPROF_FUNC;
  for (auto &level : levels_) {
    level->draw(window);
  }

//  for (auto &ent : entities_) {
//    ent->draw(window);
//  }

  using comp = std::unique_ptr<rpg::Component>;

  std::sort(components_.begin(),
            components_.end(),
            [](const comp &a, const comp &b) { return a->getPos().y < b->getPos().y; });

  for (const auto &comp : components_) {
    comp->draw(window);
//    printf("Comp %s pos: %f, %f\n", comp->name().c_str(), comp->getPos().x, comp->getPos().y);
  }
}

void rpg::ECSystem::update(sf::Time dt) {
  for (auto &item : levels_) {
    item->update(dt);
  }
  for (auto &item : entities_) {
    item->update(dt);
  }
  for (auto &item : components_) {
    item->update(dt);
  }
}

void rpg::ECSystem::handleEvent(const sf::Event &event) {
  for (auto &item : levels_) {
    item->handleEvent(event);
  }
  for (auto &item : entities_) {
    item->handleEvent(event);
  }
  for (auto &item : components_) {
    item->handleEvent(event);
  }
}

// Levels

int rpg::ECSystem::createLevel(std::string name) {

  int id = ID;

  levels_.emplace_back(std::make_unique<rpg::Level>(name, id));
  MGR->getLevel(id)->setActive(true);

//  levels_.emplace_back();
//  levels_.back()->setActive(true);

  printf("%s %s, ID: %d, total count: %zd\n", __FUNCTION__, name.c_str(), id, levels_.size());
  return id;
}

rpg::Level *rpg::ECSystem::getLevel(const int id) {
  if (levels_.empty()) {
    return nullptr;
  }

  auto result = std::find_if(levels_.begin(), levels_.end(),
                             [&](std::unique_ptr<Level> &i) { return i->id() == id; });
  if (result != levels_.end()) {
    return result->get();
  } else {
    return nullptr;
  }
}

rpg::Level *rpg::ECSystem::getLevel(std::string name) {
  if (levels_.empty()) {
    return nullptr;
  }

  auto result = std::find_if(levels_.begin(), levels_.end(),
                             [&](std::unique_ptr<Level> &i) { return i->name() == name; });
  if (result != levels_.end()) {
    return result->get();
  } else {
    return nullptr;
  }
}

// Entities

int rpg::ECSystem::createEntity(int lvl_id, std::string name) {

  int id = ID;
  if (getEntity(name) != nullptr) {
    name += "_" + std::to_string(id);
  }
  entities_.emplace_back(std::make_unique<rpg::Entity>(id, name));
  printf("%s %s, ID: %d, total count: %zd\n", __FUNCTION__, name.c_str(), id, entities_.size());
  return id;
}

rpg::Entity *rpg::ECSystem::getEntity(const int id) {
  auto result = std::find_if(entities_.begin(), entities_.end(),
                             [&](std::unique_ptr<Entity> &i) { return i->id() == id; });
  if (result != entities_.end()) {
    return result->get();
  } else {
    return nullptr;
  }
}

rpg::Entity *rpg::ECSystem::getEntity(std::string name) {

  if (entities_.empty()) {
    printf("Vector is empty - %s", name.c_str());
  }

  auto result = std::find_if(entities_.begin(), entities_.end(),
                             [&](std::unique_ptr<Entity> &i) { return i->name() == name; });
  if (result != entities_.end()) {
    return result->get();
  } else {
    return nullptr;
  }
}

// Components

rpg::Component *rpg::ECSystem::getComponent(const int id) {
  auto result = std::find_if(components_.begin(), components_.end(),
                             [&](std::unique_ptr<Component> &i) { return i->id() == id; });
  if (result != components_.end()) {
//    printf("Found %d\n", id);
    return result->get();
  } else {
    return nullptr;
  }

}

rpg::Component *rpg::ECSystem::getComponent(int owner, const std::string name) {
  auto result = std::find_if(components_.begin(), components_.end(),
                             [&](std::unique_ptr<Component> &i) { return (i->name() == name && i->owner() == owner); });
  if (result != components_.end()) {
    return result->get();
  } else {
    return nullptr;
  }
}

int rpg::ECSystem::addComponent(int owner, rpg::CType type, std::string name, float posX, float posY) {

  int id = ID;
  std::unique_ptr<rpg::Component> comp;

  switch (type) {
  case CType::image: {
    comp = std::make_unique<rpg::SimpleImage>(id, owner, name);

  }
    break;
  case CType::controller: {
    comp = std::make_unique<rpg::PlayerController>(id, owner, name);

  }
    break;

  case CType::collision: {

    auto e = getEntity(owner);
    auto posX = e->getPosition().x;
    auto posY = e->getPosition().y;
    auto size = e->getScale().y;
    rpg::Body *body = PHYS->createAABB(name, posX, posY, size);

    comp = std::make_unique<rpg::PhysicsBox>(id, owner, name, *body);

  }
    break;

  case CType::capsule: {
    comp = std::make_unique<rpg::CollisionBox>(id, owner, name);
  }
    break;

  case CType::walls: {

    auto e = getEntity(owner);
//    auto posX = e->getPosition().x;
//    auto posY = e->getPosition().y;
    auto sizeX = e->getScale().x;
    auto sizeY = e->getScale().y;
    rpg::Body *body = PHYS->createStatic(name, posX, posY, sizeX, sizeY);

    comp = std::make_unique<rpg::PhysicsBox>(id, owner, name, *body);

  }
    break;

  case CType::animatedSprite: {
    comp = std::make_unique<rpg::AnimatedSprite>(id, owner, name);
  }
    break;

//  case CType::boneAnimation: {
//    comp = std::make_unique<rpg::SkeletalAnimation>(id, owner, name);
//  }
//    break;

  case CType::sound: {
    comp = std::make_unique<rpg::Sound>(id, owner, name);
  }
    break;

  case CType::state: {
    comp = std::make_unique<rpg::ActorState>(id, owner, name);
  }
    break;

  case CType::commandlistener: {
    comp = std::make_unique<rpg::ActorController>(id, owner, name);
  }
    break;

  }

//  comp->setActive(true);
  components_.push_back(std::move(comp));
  getEntity(owner)->addComponent(id);
  return id;
}

void rpg::ECSystem::clear() {

//  for(auto & comp : components_){
//    comp.reset();
//  }
//
//  for(auto & ent : entities_){
//    ent.reset();
//  }
//
//  for(auto & lvl : levels_){
//    lvl.reset();
//  }

  components_.clear();
  entities_.clear();
  levels_.clear();

  printf("%s\n", __FUNCTION_NAME__);
}
