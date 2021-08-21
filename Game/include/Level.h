#pragma once
#include "precompiled.h"
#include "Entity.h"
#include "ECSystem.h"
#include "TileMap.h"
#include "Physics.h"
#include "Object.h"

namespace rpg {

class Level : public Object {

public:
  explicit Level(std::string name, int id);
  ~Level(){onDestroy();}

  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

  // Objects API
  void attach_entity(int id){entities_.push_back(id);}
  void remove_entity(int id);
  void load_tilemap(std::string path);

  // Getters and setters
  int parent() const { return parent_; }
  std::vector<int> levels() { return levels_; }
  std::vector<int> entities() { return entities_; }

  int spawnActor(std::string name, float posX, float posY);
  int spawnAnimatedActor(std::string name, float posX, float posY);

private:
  void onActivate() override;
  void onDestroy() override {};

private:
  int parent_ = 0;          // Parent Level id, "0" if not exist
  std::vector<int> levels_; //Nested levels
  std::vector<int> entities_;
  std::unique_ptr<rpg::TileMap> tile_map_;
};

}
