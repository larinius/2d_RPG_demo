#include "precompiled.h"
#include <Engine.h>
#include <Components/SimpleImage.h>
#include "Layer.h"
#include "Components/Sound.h"
#include <AnimatedSprite.h>
#include "Level.h"
#include "AI.h"

rpg::Layer::Layer(std::string name,
                  int id,
                  const ldtk::Layer &layer,
                  const std::vector<ldtk::Tile> &tiles_vector) : Object(name, id),
                                                                 layer(&layer),
                                                                 tiles_vector(&tiles_vector) {
}

void rpg::Layer::onActivate() {

  if (name_ == "Entities") {
    loadEntities();
//    loadCollisions();
  } else if (name_ == "Wall_overlays") {
    loadCollisions();
    loadTiles();
  } else {
    loadTiles();
  }

}

void rpg::Layer::draw(sf::RenderWindow *window) {
  sf::RenderStates states;
  states.transform.scale(4, 4);
  states.texture = &tileset_texture;
  window->draw(tilemap, states);
}

void rpg::Layer::loadEntities() {

  for (const auto e : layer->allEntities()) {
    printf("%s\n", e.c_str());
    for (const auto &e : layer->getEntities(e)) {

      if (e.getName() == "Player") {
        sf::Vector2f pos(e.getPosition().x * LDtkSCALEDGRID, e.getPosition().y * LDtkSCALEDGRID);
        AISYS->spawnPlayer("Player", "Level One", pos.x, pos.y);
      }

      if (e.getName() == "Monster") {
        auto scale = e.getField<float>("scale").value();

        sf::Vector2f pos(e.getPosition().x * LDtkSCALEDGRID, e.getPosition().y * LDtkSCALEDGRID);
        AISYS->spawnActor("Zombie", "Level One", pos.x, pos.y, scale);
      }

      if (e.getName() == "Trap") {
        sf::Vector2f pos(e.getPosition().x * LDtkSCALEDGRID, e.getPosition().y * LDtkSCALEDGRID);
        auto scale = e.getField<float>("scale").value();
        AISYS->spawnActor("Fire", "Level One", pos.x, pos.y, scale);
      }
    }
  }
}

void rpg::Layer::loadTiles() {
  auto p = "Assets/maps/" + layer->getTileset().path;
  tileset_texture.loadFromFile(p);

  tilemap.resize(tiles_vector->size() * 4);  // each tile has 4 vertices
  tilemap.setPrimitiveType(sf::PrimitiveType::Quads);
  int i = 0;
  for (const auto &tile : *tiles_vector) {
    for (int j = 0; j < 4; ++j) {
      // set the position and texture coordinate of each vertex
      // flips are already done, you don't have to do it manually
      tilemap[i * 4 + j].position.x = tile.vertices[j].pos.x;
      tilemap[i * 4 + j].position.y = tile.vertices[j].pos.y;
      tilemap[i * 4 + j].texCoords.x = static_cast<float>(tile.vertices[j].tex.x);
      tilemap[i * 4 + j].texCoords.y = static_cast<float>(tile.vertices[j].tex.y);
    }
    i++;

    float posX = tile.position.x * LDtkSCALEDGRID;
    float posY = tile.position.y * LDtkSCALEDGRID;

    int tileId = std::stoi(std::to_string(1000 + (int) posX / (int) GRID) +
        std::to_string(1000 + (int) posY / (int) GRID)
    );
    auto node = std::make_unique<NavNode>();
    node->tag = "Tiles";
    node->id = {tile.position.x, tile.position.y};
    node->tileId = tileId;
    node->position = mathfu::Vector<float, 2>(posX, posY);
    node->size = mathfu::Vector<float, 2>(GRID, GRID);
    node->navigable = true;

    NAVSYS->addNode(std::move(node));

  }

}

void rpg::Layer::loadCollisions() {

  auto level = MGR->getLevel("Level One")->id();
  auto entity = MGR->createEntity(level, "Walls");
  auto ent = MGR->getEntity(entity);
  ent->setSize(GRID);
  ent->setOrigin(0.f, 0.f);
  ent->setScale(0.9f, 0.9f);
  ent->setPosition(0.f, 0.f);

//  printf("Loading Collisions in  %s, %d\n", name.c_str(), id);

  for (const auto e : layer->allTiles()) {
//    printf("Tile pos: %d %d\n", e.position.x, e.position.y);

//    float posX = (e.position.x / 16) * GRID + (0.5 * GRID);
//    float posY = (e.position.y / 16) * GRID + (0.5 * GRID);

    float posX = e.position.x * LDtkSCALEDGRID;
    float posY = e.position.y * LDtkSCALEDGRID;

    int tileId = std::stoi(
//        std::to_string((int) ((1000 + (e.position.x + GRID / 2) / GRID))) +
//            std::to_string((int) (1000 + (e.position.y + GRID / 2) / GRID))
        std::to_string(1000 + (int) posX / (int) GRID) +
            std::to_string(1000 + (int) posY / (int) GRID)
    );
//    float posX = e.position.x;
//    float posY = e.position.y;



//    float posX = (e.position.x * 4.f) - 300.f;
//    float posY = (e.position.y * 4.f) - 20.f;

//    sf::Vector2f pos(posX, posY);
//    printf("Wall tile: %f, %f\n", posX, posY);
    auto
        collision = MGR->addComponent(entity, CType::walls, "CollisionBox", posX + (GRID * 0.5f), posY + (GRID * 0.5f));

    auto node = std::make_unique<NavNode>();
    node->tag = "Walls";
    node->id = {e.position.x, e.position.y};
    node->tileId = tileId;
    node->position = mathfu::Vector<float, 2>(posX, posY);
    node->size = mathfu::Vector<float, 2>(GRID, GRID);
    node->navigable = false;

    NAVSYS->addNode(std::move(node));

  }

}
