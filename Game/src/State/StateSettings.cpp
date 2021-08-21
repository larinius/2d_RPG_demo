#include "precompiled.h"
#include "State/StateSettings.h"
#include "Helpers.h"
#include "Engine.h"
#include "Globals.h"
#include "Macroses.h"

using namespace rpg;

StateSettings::StateSettings(GameState state) : State(state) {}

void StateSettings::draw() {
//  context.gui->draw();
//  GUI.draw();
}

void StateSettings::update(sf::Time dt) {
  //textEffectTime += dt;

  //if (textEffectTime >= sf::seconds(0.5f))
  //{
  //	isShowtext = !isShowtext;
  //	textEffectTime = sf::Time::Zero;
  //}
}

void StateSettings::handleEvent(const sf::Event &event) {

//  GUI->handleEvent(event);

  if (event.type == sf::Event::KeyReleased) {
//    requestStackPop();
//    requestStackPush(StateID::Play);
//    switchState(StateID::Play);
  }

}

void StateSettings::onActivate() {
  std::cout << "Settings mode activted!" << "\n";
//  initGUI();
  rpg::Gui::settingsGui();
}

void StateSettings::initGUI() {
  //do init things, will run only once
//  std::cout << "Initialisation done" << "\n";
//  context.gui->setTarget(*context.window);
//  const std::string menuFile = "D:\\Dev\\Src\\Cpp\\RPG\\Assets\\gui\\form_settings.txt";
//  context.gui->loadWidgetsFromFile(menuFile);
//  GUI->loadWidgetsFromFile(rpg::helper::loadJson("Assets/json/config.json", "gui_settings"));
//
//  GUI->get<tgui::Widget>("windowSettings")->showWithEffect(
//      tgui::ShowAnimationType::SlideFromLeft, 200);

//  gui.get<tgui::Button>("buttonStart")->onPress(&State::switchState, this, StateID::Play);
//  gui.get<tgui::Button>("buttonSettings")->onPress(&State::switchState, this, StateID::Settings);
//  gui.get<tgui::Button>("buttonQuit")->onPress(&State::switchState, this, rpg::StateID::Quit);
}
void StateSettings::onDestroy() {
  rpg::Gui::clear();
}
StateSettings::~StateSettings() {
  onDestroy();
}
