#pragma once
#include "precompiled.h"
#include "Object.h"

namespace rpg{

class Gui : public Object {
public:
  Gui(std::string name, int id);
  ~Gui()  {onDestroy();}

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt)  override;
  void handleEvent(const sf::Event &event)  override;

  void setCursor(std::string path);

  tgui::GuiSFML* get(){return &gui_;}

  //GUI forms
  static void menuGui();
  static void settingsGui();
  static void gameGui();

  //Signnal handlers for (callbacks)
  static void mainMenuBtn();
  static void startBtn();
  static void settingsBtn();
  static void quitBtn();
  static void clear();
  static void onHoverBtn();

  //Gui Sounds
  void addSound(std::string name, std::string path, float volume=100.f);
  static void playSound(std::string name);
  static GuiSoundObject* getSound(std::string name);

private:
  void onActivate() override;
  void onDestroy() override {};

private:
  std::string assets_path_;
  static tgui::GuiSFML gui_;
  static tgui::Theme theme_;
  sf::Sprite cursor_;
  sf::Texture cursor_texture_;
  sf::RenderWindow *window_;

  static inline std::deque<std::unique_ptr<rpg::GuiSoundObject>> sounds_{};
  sf::Clock clock_;
};

};
