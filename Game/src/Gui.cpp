#include "precompiled.h"
#include "Gui.h"
#include "Globals.h"
#include "Helpers.h"
#include "Engine.h"
#include "Macroses.h"
#include "State/StateMachine.h"

tgui::GuiSFML rpg::Gui::gui_;

rpg::Gui::Gui(std::string name, int id) : Object(name, id) {

}

void rpg::Gui::onActivate() {
  window_ = RWIND;
  std::string const &path = "config.json";
  assets_path_ = rpg::helper::loadJson("Assets/json/config.json", "assets");
  auto cursor_file = rpg::helper::loadJson("Assets/json/config.json", "cursor");
  setCursor(cursor_file);
  gui_.setTarget(*window_);

  addSound("Hover", "Assets/gui/sounds/Menu_select_item.ogg", 100.f);
  addSound("Click", "Assets/gui/sounds/Button_click_2.ogg", 100.f);


}

void rpg::Gui::draw(sf::RenderWindow *window) {
  gui_.draw();
  window->draw(cursor_);
}

void rpg::Gui::update(sf::Time dt) {
  cursor_.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window_)));
}

void rpg::Gui::handleEvent(const sf::Event &event) {
  gui_.handleEvent(event);
}

void rpg::Gui::setCursor(std::string path) {

  window_->setMouseCursorVisible(false); // Hide cursor
  cursor_texture_.loadFromFile(path);
  cursor_texture_.setSmooth(true);
  cursor_.setTexture(cursor_texture_);
}

// Gui loaders for GameStates

void rpg::Gui::menuGui() {

  auto path = rpg::helper::loadJson("Assets/json/config.json", "gui_title");
  gui_.loadWidgetsFromFile(path);

  gui_.get<tgui::Button>("buttonStart")->onPress(&Gui::startBtn);
  gui_.get<tgui::Button>("buttonSettings")->onPress(&Gui::settingsBtn);
  gui_.get<tgui::Button>("buttonQuit")->onPress(&Gui::quitBtn);

  gui_.get<tgui::Button>("buttonStart")->onMouseEnter(&Gui::onHoverBtn);
  gui_.get<tgui::Button>("buttonSettings")->onMouseEnter(&Gui::onHoverBtn);
  gui_.get<tgui::Button>("buttonQuit")->onMouseEnter(&Gui::onHoverBtn);

  printf("%s\n", __FUNCTION_NAME__);
}

void rpg::Gui::settingsGui() {
  gui_.loadWidgetsFromFile(rpg::helper::loadJson("Assets/json/config.json", "gui_settings"));

  gui_.get<tgui::Widget>("windowSettings")->showWithEffect(
      tgui::ShowAnimationType::SlideFromLeft, 200);

//  gui.get<tgui::Button>("buttonStart")->onPress(&State::switchState, this, StateID::Play);
//  gui.get<tgui::Button>("buttonSettings")->onPress(&State::switchState, this, StateID::Settings);
//  gui.get<tgui::Button>("buttonQuit")->onPress(&State::switchState, this, rpg::StateID::Quit);
}

void rpg::Gui::gameGui() {
  //do init things, will run only once
  std::cout << "Initialisation done" << "\n";
//  context.gui->setTarget(*context.window);
//  const std::string menuFile = "D:\\Dev\\Src\\Cpp\\RPG\\Assets\\gui\\form_play.txt";
//  context.gui->loadWidgetsFromFile(menuFile);
  gui_.loadWidgetsFromFile(rpg::helper::loadJson("Assets/json/config.json", "gui_play"));

  gui_.get<tgui::Button>("buttonMenu")->onPress(&Gui::mainMenuBtn);
  gui_.get<tgui::Button>("buttonSettings")->onPress(&Gui::settingsBtn);
  gui_.get<tgui::Button>("buttonQuit")->onPress(&Gui::quitBtn);
}

// Button callbacks

void rpg::Gui::mainMenuBtn() {
  printf("Return to main menu Button\n");
  rpg::StateMachine::switchState(GameState::Menu);
  printf("%s\n", __FUNCTION_NAME__);
}

void rpg::Gui::startBtn() {
  playSound("Click");
  printf("Start Button\n");
  rpg::StateMachine::switchState(GameState::Play);
  printf("%s\n", __FUNCTION_NAME__);
}

void rpg::Gui::settingsBtn() {
  playSound("Click");
  printf("Settings Button\n");
  rpg::StateMachine::switchState(GameState::Settings);
}

void rpg::Gui::quitBtn() {
  playSound("Click");
  printf("Quit Button\n");
  rpg::StateMachine::quit();
}



void rpg::Gui::clear() {
  //gui_.removeAllWidgets(); //todo Bug?
  printf("%s\n", __FUNCTION_NAME__);
}

void rpg::Gui::addSound(std::string name, std::string path, float volume) {

  auto so = std::make_unique<rpg::GuiSoundObject>();
  so->name = name;
  so->path = path;

  if (!so->buffer.loadFromFile(so->path)) {
    printf("Sound Load Error: %s\n", path.c_str());
  }

  so->sound.setBuffer(so->buffer);
  so->sound.setVolume(volume);

  sounds_.push_front(std::move(so));
}

rpg::GuiSoundObject *rpg::Gui::getSound(std::string name) {
  auto result = std::find_if(sounds_.begin(), sounds_.end(),
                             [&](const std::unique_ptr<GuiSoundObject> &i) { return i->name == name; });

  return result != sounds_.end() ? result->get() : nullptr;
}

void rpg::Gui::playSound(std::string name) {
  getSound(name)->sound.play();
}

void rpg::Gui::onHoverBtn() {
  playSound("Hover");
}
