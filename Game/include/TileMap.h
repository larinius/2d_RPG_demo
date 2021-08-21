#pragma once
#include "precompiled.h"
#include <LDtk_Headers.h>
#include "Layer.h"
#include "Object.h"

namespace rpg {

class TileMap : public Object{
public:
  TileMap(int id, std::string name, int owner, std::string path);
  TileMap(const TileMap &) = delete;
  TileMap& operator=(const TileMap &) = delete;
  ~TileMap(){onDestroy();}

  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

private:
  void onActivate() override;
  void onDestroy() override;

  const int owner_;
  std::string path_;
  ldtk::World world_;
//  std::map<int, std::string> layer_list;
  std::vector<std::unique_ptr<Layer>> layers_;
};

}
