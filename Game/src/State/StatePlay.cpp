#include "precompiled.h"
#include "State/StatePlay.h"
#include "State/StateMachine.h"
#include "Helpers.h"
#include "Engine.h"
#include "ECSystem.h"
#include "Level.h"
#include "Components/SimpleImage.h"
#include "Components/Component.h"
#include "Tag.h"
#include "Globals.h"
#include "Macroses.h"
#include "Audio.h"

rpg::StatePlay::StatePlay(GameState state) : State(state) {}

void rpg::StatePlay::draw(sf::RenderWindow *window) {
//  MGR.draw();
//  GUI.draw();
}

void rpg::StatePlay::update(sf::Time dt) {
}

void rpg::StatePlay::handleEvent(const sf::Event &event) {
//  GUI->handleEvent(event);

  // If any key is pressed, trigger the next screen
  if (event.type == sf::Event::KeyReleased) {
//    requestStackPop();
//    requestStackPush(StateStack::Menu);
//    std::cout << "Key Pressed" << "\n";
  }
}

void rpg::StatePlay::onActivate() {

  AUDIO->addAudio("Assets/music/level.ogg", "Level");
  AUDIO->playAudio("Level", true, 50.f);
//  MGR->clear();
  rpg::helper::printLog("Play mode active");
  rpg::Gui::gameGui();
  PHYS->setActive(true);
  // Creating scene objects
  auto level = MGR->createLevel("Level One");
  MGR->getLevel(level)->spawnActor("Pointer", 500.f, 500.f);
  MGR->getEntity("Pointer")->setScale(0.1, 0.1);

//  auto entity = MGR.createEntity(level, "Player");
//  auto component1 = MGR.create_component(entity, CType::image, "Component image");
//  dynamic_cast<rpg::SimpleImage&>(MGR.getComponent(component1)).set_image("Assets/Sprites/Player_front.png");
//  auto component2 = MGR.create_component(entity, CType::controller, "Controller");

//  rpg::Component *c = &MGR.getComponent(component1);
//  printf("%s", c->name().c_str());

/* Tag test */
//  rpg::TagFactory::registerTag("Wall");
//  rpg::TagFactory::registerTag("Box");
//  auto tag_id = rpg::TagFactory::registerTag("Door");
//  rpg::TagFactory::registerTag("Enemy");
//
//  printf("Find tag by id %s, %d, %d\n",
//         rpg::TagFactory::getTag(tag_id).alias.c_str(), tag_id, rpg::TagFactory::getTag(tag_id).id);
//
//  for (const auto& t : rpg::TagFactory::getTags()) {
//    printf("TAG: %s, %d, %d\n", t.alias.c_str(), t.id, t.isActive);
//  }


}

void rpg::StatePlay::onDestroy() {
  printf("Destroy Play state\n");
  AUDIO->stopAudio();
  rpg::Gui::clear();
  MGR->clear();
  PHYS->setActive(false);
  PHYS->clear();
  rpg::helper::Id::reset();
  printf("%s\n", __FUNCTION_NAME__);
}

rpg::StatePlay::~StatePlay() {
  onDestroy();
  printf("%s\n", __FUNCTION_NAME__);
}

//void rpg::StatePlay::initGUI() {
//  //do init things, will run only once
////  std::cout << "Initialisation done" << "\n";
////  context.gui->setTarget(*context.window);
////  const std::string menuFile = "D:\\Dev\\Src\\Cpp\\RPG\\Assets\\gui\\form_play.txt";
////  context.gui->loadWidgetsFromFile(menuFile);
////  GUI->loadWidgetsFromFile(rpg::helper::loadJson("Assets/json/config.json", "gui_play"));
//
////  GUI->get<tgui::Button>("buttonStart")->onPress(&State::switchState, this, StateID::Play);
////  GUI->get<tgui::Button>("buttonSettings")->onPress(&State::switchState, this, StateID::Settings);
////  GUI->get<tgui::Button>("buttonQuit")->onPress(&State::switchState, this, rpg::StateID::Quit);
//}
