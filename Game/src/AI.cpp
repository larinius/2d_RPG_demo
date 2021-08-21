#include "AI.h"
#include "Engine.h"
#include "Sound.h"
#include "SimpleImage.h"
#include "AnimatedSprite.h"
#include "ActorController.h"
#include "PhysicsBox.h"

rpg::AI::AI(std::string name, int id) : Object(name, id) {

}

void rpg::AI::draw(sf::RenderWindow *window) {

}

void rpg::AI::update(sf::Time dt) {

}

void rpg::AI::handleEvent(const sf::Event &event) {

}

void rpg::AI::onActivate() {

}

void rpg::AI::onDestroy() {

}

void rpg::AI::spawnPlayer(std::string player_name, std::string level_name, float posX, float posY) {

  sf::Vector2f pos(posX, posY);

  auto level = MGR->getLevel(level_name)->id();
  auto player = MGR->createEntity(level, player_name);
  auto ent = MGR->getEntity(player);
  ent->setPosition(pos);
  ent->setScale(0.9f, 0.9f);

  auto state = MGR->addComponent(player, CType::state, "ActorState");

  auto collision = MGR->addComponent(player, CType::collision, "CollisionBox", pos.x, pos.y);

  auto sound_id = MGR->addComponent(player, CType::sound, "Sound");
  dynamic_cast<rpg::Sound *>(MGR->getComponent(sound_id))->addSound("Punch",
                                                                    "Assets/sounds/Sharp_Punch.ogg",
                                                                    false,
                                                                    100.f);
  dynamic_cast<rpg::Sound *>(MGR->getComponent(sound_id))->addSound("Ahh",
                                                                    "Assets/sounds/Female_ahh.ogg",
                                                                    false,
                                                                    100.f);
  dynamic_cast<rpg::Sound *>(MGR->getComponent(sound_id))->addSound("Ohh",
                                                                    "Assets/sounds/Female_ohh.ogg",
                                                                    false,
                                                                    100.f);
  dynamic_cast<rpg::Sound *>(MGR->getComponent(sound_id))->addSound("Yay",
                                                                    "Assets/sounds/Female_yay.ogg",
                                                                    false,
                                                                    100.f);

  auto caps = MGR->addComponent(player, CType::capsule, "Capsule");
  auto anim = MGR->addComponent(player, CType::animatedSprite, "AnimatedSprite");
  dynamic_cast<rpg::AnimatedSprite *>(MGR->getComponent(anim))->setPath("Assets/Sprites/Warrior_girl/");

  auto controller = MGR->addComponent(player, CType::controller, "Controller");

  rpg::Actor actor;
  actor.entity = ent;
  actor.npcId = player;
  actor.npcName = "Player";

  npcList_.push_back(actor);
  ent->setActive(true);

}

void rpg::AI::spawnActor(std::string actor_name, std::string level_name, float posX, float posY, float scale) {

  sf::Vector2f pos(posX, posY);

  auto ent_id = MGR->createEntity(MGR->getLevel("Level One")->id(), actor_name);
  auto sprite = MGR->addComponent(ent_id, CType::animatedSprite, "AnimatedSprite");
  auto caps = MGR->addComponent(ent_id, CType::capsule, "Capsule", pos.x, pos.y);
  dynamic_cast<rpg::AnimatedSprite *>(MGR->getComponent(sprite))->setPath("Assets/Sprites/" + actor_name + "/");

  auto ent = MGR->getEntity(ent_id);
//  ent->addComponent(sprite);
  ent->setScale(scale, scale);
  ent->setOrigin(ent->getSize() / 2, ent->getSize() / 2);
  ent->setPosition(pos);

  if (actor_name == "Zombie") {
    rpg::Actor actor;
    actor.entity = ent;
    actor.npcId = ent_id;
    actor.npcName = actor_name;
    npcList_.push_back(actor);

    COMMAND->subscribe(ent_id);

    auto state = MGR->addComponent(ent_id, CType::state, "ActorState");
    auto listener = MGR->addComponent(ent_id, CType::commandlistener, "CommandComponent");
    auto collision = MGR->addComponent(ent_id, CType::collision, "CollisionBox", pos.x, pos.y);
    dynamic_cast<PhysicsBox *>(MGR->getComponent(ent_id, "CollisionBox"))->setVelocity(0, 0, 0);


    dynamic_cast<ActorController *>(MGR->getComponent(listener))->setType(actor_name);
//    setBehavior(ent_id, rpg::Behavior::idle);
//    COMMAND->dispatchCommand(ent_id, Action::set_behavior, rpg::Behavior::idle);
//    COMMAND->dispatchCommand(ent_id, Action::set_behavior, rpg::Behavior::roaming);
    COMMAND->dispatchCommand(ent_id, Action::set_behavior, rpg::Behavior::aggressive);
  }

  ent->setActive(true);
}

void rpg::AI::setBehavior(int npc_id, rpg::Behavior behavior) {
//
//  switch (behavior) {
//  case Behavior::idle:COMMAND->dispatchCommand(npc_id, Action::set_behavior, behavior);
//    break;
//  case Behavior::roaming:COMMAND->dispatchCommand(npc_id, Action::set_behavior, behavior);
//    break;
//  case Behavior::aggressive:COMMAND->dispatchCommand(npc_id, Action::set_behavior, behavior);
//    break;
//  default:break;
//  }
}
