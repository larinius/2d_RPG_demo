#pragma once
#include "precompiled.h"
#include "Object.h"
#include "Entity.h"

namespace rpg {

/**
 * More complex NPC behavior algorithms based on simple Actions
 */



class AI : public Object {
public:
  AI(std::string name, int id);

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

private:
  void onActivate() override;
  void onDestroy() override;

public:
  void spawnPlayer(std::string name, std::string level, float posX, float posY);
  void spawnActor(std::string name, std::string level, float posX, float posY, float scale);

private:
  void setBehavior(int npc_id, Behavior behavior);
  void roaming(int range);
  void idle(int npc_id);
  void attack(int npc_id);
  void follow(int npc_id);
  void chase(int npc_id);

private:
  std::deque<rpg::Actor> npcList_;
};

}
