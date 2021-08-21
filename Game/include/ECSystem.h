#pragma once
#include "precompiled.h"
#include "Object.h"
#include "Component.h"

namespace rpg {

class Engine;
class Level;
class Entity;
class Component;

class ECSystem : public Object {

public:
  ECSystem(std::string name, int id);
  ~ECSystem(){onDestroy();}

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

  // Levels API
  int createLevel(std::string name);
  Level *getLevel(int id);
  Level *getLevel(std::string name);

  // Entities API
  int createEntity(int lvl_id, std::string name);
  Entity* getEntity(int id);
  Entity* getEntity(std::string name);

  // Components API
  int addComponent(int owner, rpg::CType type, std::string name, float posX = 0.f, float posY = 0.f);
  Component *getComponent(int id);
  Component *getComponent(int owner, std::string name);

  void clear();

private:
  void onActivate() override {};
  void onDestroy() override {}

  std::vector<std::unique_ptr<Level>> levels_;
  std::vector<std::unique_ptr<Entity>> entities_;
  std::vector<std::unique_ptr<Component>> components_;
};

}
