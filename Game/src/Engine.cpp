#include "precompiled.h"
#include "Macroses.h"
#include "Engine.h"
#include "Globals.h"
#include "Gui.h"
#include "State/StateMachine.h"
#include "Audio.h"
#include "CollisionSystem.h"
#include "CollisionSystem.h"
#include "AI.h"

namespace rpg {

Engine::Engine()
    : window_(rpg::videoMode, rpg::gameTitle, sf::Style::Close)
    {
  es = new Events();

  window_.setKeyRepeatEnabled(false);
  window_.setVerticalSyncEnabled(true);

  registerStates();
  wind_ = &window_;

}

int Engine::Run() {

  if (!Engine::init())
    return 1;

  auto state = dynamic_cast<StateMachine*>(get("StateMachine"));

  sf::Clock clock;
  sf::Time lastUpdateTime = sf::Time::Zero;

  while (window_.isOpen()) {
    sf::Time dt = clock.restart();
    lastUpdateTime += dt;
    while (lastUpdateTime > frameTime) {
      lastUpdateTime -= frameTime;

      handleEvent();
      update(frameTime);

      if (state->isQuit()) {
        std::cout << "Quit" << std::endl;
        window_.close();
      }

    }

    draw();
  }

  return 0;
}

void Engine::handleEvent() {
  sf::Event event;
  while (window_.pollEvent(event)) {
//    stack_.handleEvent(event);

    for (const auto &object: engine_objects_) {
      object->handleEvent(event);
    }

    if (event.type == sf::Event::Closed)
      window_.close();

    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::End) {
        InternalProfiler::aggregateEntries();
        std::cout << "The latest internal profiler stats:\n"
                  << InternalProfiler::stats << std::endl;
      }
    }
  }
}

void Engine::update(sf::Time dt) {
//  IPROF_FUNC;
//  stack_.update(dt);
  for (const auto &object: engine_objects_) {
    object->update(dt);
  }
}

void Engine::draw() {
//  IPROF_FUNC;
  window_.clear();
//  stack_.draw();
  for (const auto &object: engine_objects_) {
    object->draw(&window_);
  }
  window_.setView(window_.getDefaultView());
  window_.display();
}

void Engine::registerStates() {
//  stack_.registerState<StateTitle>(StateID::Title);
  //stack_.registerState<GameModeMenu>(StateID::Menu);
//  stack_.registerState<StatePlay>(StateID::Play);
  //stack_.registerState<GameModePause>(StateID::Pause);
//  stack_.registerState<StateSettings>(StateID::Settings);
  //stack_.registerState<GameModeOver>(StateID::GameOver, "Mission Failed!");
  //stack_.registerState<GameOverState>(StateID::MissionSuccess, "Mission Successful!");
}

bool Engine::init(std::string const &path) {
  Engine::assets_path_ = rpg::helper::loadJson("Assets/json/config.json", "assets");

  if (Engine::assets_path_.empty()) {
    rpg::helper::printLog("Failed to initialize");
    return false;
  }

  engine_objects_.emplace_back(std::make_unique<ECSystem>("ECSManager", ID));
  engine_objects_.emplace_back(std::make_unique<Physics>("Physics", ID));
  engine_objects_.emplace_back(std::make_unique<NavSystem>("Navigation", ID));
  engine_objects_.emplace_back(std::make_unique<Gui>("Gui", ID));
  engine_objects_.emplace_back(std::make_unique<Audio>("Audio", ID));
  engine_objects_.emplace_back(std::make_unique<AI>("AI", ID));
  engine_objects_.emplace_back(std::make_unique<CommandQueue>("CommandQueue", ID));
  engine_objects_.emplace_back(std::make_unique<CollisionSystem>("Collisions", ID));

  engine_objects_.emplace_back(std::make_unique<StateMachine>("StateMachine", ID));

  get("ECSManager")->setActive(true);
  get("Physics")->setActive(true);
  get("Navigation")->setActive(true);
  get("Gui")->setActive(true);
  get("Audio")->setActive(true);
  get("Collisions")->setActive(true);
  get("CommandQueue")->setActive(true);
  get("AI")->setActive(true);

  get("StateMachine")->setActive(true);

  return true;
}

Object * Engine::get(std::string name) {
  if (Engine::engine_objects_.empty()) {
    return nullptr;
  }

  auto result = std::find_if(engine_objects_.begin(), engine_objects_.end(),
                             [&](std::unique_ptr<Object> &i) { return i->name() == name; });
  if (result != engine_objects_.end()) {
    return result->get();
  } else {
    return nullptr;
  }

}

}
