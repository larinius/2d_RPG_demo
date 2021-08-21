#pragma once
#include "precompiled.h"
#include "Object.h"
#include <LDtkLoader/World.hpp>

namespace rpg {

class Layer : public Object {
public:
  Layer(std::string name, int id, const ldtk::Layer &layer, const std::vector<ldtk::Tile> &tiles_vector);
  ~Layer(){onDestroy();}

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override {};
  void handleEvent(const sf::Event &event) override {};

private:
  void onActivate() override;
  void onDestroy() override {};

  void loadEntities();
  void loadTiles();
  void loadCollisions();

private:
  const ldtk::Layer *layer;
  const std::vector<ldtk::Tile> *tiles_vector;
  sf::Texture tileset_texture;
  sf::VertexArray tilemap;
};

}
