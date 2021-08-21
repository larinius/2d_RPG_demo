#pragma once
#include "precompiled.h"
#include "Events.h"
#include "Level.h"
#include "State/StateTitle.h"
#include "State/StatePlay.h"
#include "State/StateSettings.h"
#include "Physics.h"
#include "NavSystem.h"
#include "Object.h"
#include "Gui.h"

namespace rpg {



class Engine {
public:
  Engine();
  Engine(Engine const &) = delete;
  Engine &operator=(Engine const &) = delete;
  ~Engine() = default;

  int Run();
//  void setCursor(std::string path);

private:
  void handleEvent();
  void draw();
  void update(sf::Time dt);

  void updateStatistics(sf::Time dt);
  void registerStates();

public:
//  static const sf::Time frameTime;

  // Public API for all engine classes
  static sf::RenderWindow *window() { return wind_; }
//  rpg::StateStack &stack() { return stack_; }
//  rpg::ECSystem &mgr() { return mgr_; }
//  tgui::GuiSFML &gui() { return gui_; }
//  tgui::Theme &theme() { return theme_; };
//  rpg::Physics *physics();
//  rpg::NavSystem &navsystem() { return navsystem_; };
  static Object * get(std::string name);

private:
  bool init(std::string const &path = "config.json");
  rpg::Events *es;
  sf::RenderWindow window_;
  static inline sf::RenderWindow *wind_; //return it
//  rpg::StateStack stack_;
//  rpg::ECSystem mgr_;
  std::string assets_path_;
//  tgui::GuiSFML gui_;
//  tgui::Theme theme_;
//  rpg::Physics physics_;
//  rpg::NavSystem navsystem_;

  static inline std::vector<std::unique_ptr<Object>> engine_objects_;

//  sf::Sprite cursor_;
//  sf::Texture texture_;

};



}
