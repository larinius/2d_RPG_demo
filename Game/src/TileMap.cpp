#include "precompiled.h"
#include "TileMap.h"
#include "Helpers.h"
#include "Globals.h"
#include "Macroses.h"

rpg::TileMap::TileMap(int id, std::string name, int owner, std::string path) : Object(name, id),
                                                                               owner_(owner), path_(path) {}

void rpg::TileMap::onActivate() {
//  auto p = "Assets/maps/Demo_level.ldtk";
  world_.loadFromFile(path_);
  const auto& level = world_.getLevel("Entrance");
  const auto& layers = level.allLayers();

  for(const auto &l : layers){
//    printf("%s\n", l.getName().c_str());

    auto id = ID;
    auto name = l.getName();
    const auto & layer = level.getLayer(name.c_str());
    const auto & tiles_vector = layer.allTiles();

    layers_.emplace_back(std::make_unique<Layer>(name, id, layer, tiles_vector));

  }

  for(const auto &l : layers_){
//    printf("%s, %d\n", l->name.c_str(), l->id);
    l->setActive(true);
  }

  int i = 0;
}

void rpg::TileMap::onDestroy() {

}

void rpg::TileMap::draw(sf::RenderWindow *window) {
//  for(const auto &l : layers_) {
//    l->draw();
//  }
  //1
  layers_[5]->draw(window);
  layers_[4]->draw(window);
  layers_[3]->draw(window);
  layers_[2]->draw(window);
  layers_[1]->draw(window);
  layers_[0]->draw(window);
}

void rpg::TileMap::update(sf::Time dt) {

}

void rpg::TileMap::handleEvent(const sf::Event &event) {

}
