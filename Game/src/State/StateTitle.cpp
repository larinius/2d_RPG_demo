#include "precompiled.h"
#include "Globals.h"
#include "State/State.h"
#include "State/StateTitle.h"
#include "State/StateMachine.h"
#include "Helpers.h"
#include "Engine.h"
#include "Macroses.h"
#include "Gui.h"
#include "Audio.h"

rpg::StateTitle::StateTitle(GameState state) : State(state) {}

void rpg::StateTitle::draw(sf::RenderWindow *window) {
//  GUI.draw();
}

void rpg::StateTitle::update(sf::Time dt) {
  //textEffectTime += dt;

  //if (textEffectTime >= sf::seconds(0.5f))
  //{
  //	isShowtext = !isShowtext;
  //	textEffectTime = sf::Time::Zero;
  //}

}

void rpg::StateTitle::handleEvent(const sf::Event &event) {

//  GUI->handleEvent(event);

  // If any key is pressed, trigger the next screen
  if (event.type == sf::Event::KeyReleased) {
//    requestStackPop();
//    requestStackPush(StateID::Play);
//    switchState(StateID::Play);
    std::cout << "Key Pressed" << "\n";
  }
}

void rpg::StateTitle::onActivate() {
  printf("Title mode activated!\n");
  rpg::Gui::menuGui();
  AUDIO->addAudio("Assets/music/intro.ogg", "Intro");
  AUDIO->playAudio("Intro", true, 100.f);

}

void rpg::StateTitle::onDestroy() {
  AUDIO->stopAudio();
  rpg::Gui::clear();
}

rpg::StateTitle::~StateTitle() {
 onDestroy();
}
