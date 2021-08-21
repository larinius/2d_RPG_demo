#pragma once
#include "precompiled.h"
#include "Component.h"
#include "CommandQueue.h"
#include "Structs.h"

namespace rpg {

class ActorController : public Component, public CommandListener, public sf::Transformable {
public:
  ActorController(int id, int owner, std::string name);

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;
  void onCommand(Command command) override;

private:
  void onActivate() override;
  void onDestroy() override;

  // Movement
private:
  void updateActions(rpg::Action action, sf::Time dt);
  void moveToPoint(float posX, float posY);
  void setRoute(std::deque<mathfu::Vector<float, 2>> route);
  void moveByRout(std::deque<mathfu::Vector<float, 2>>, float speed);
  void keyMove(float destX, float destY, float speed){};

  // Behavior
public:
  void loadBehaviors(std::string path);
  void setType(std::string type){actorType_ = type;}
  void switchBehavior(Behavior behavior);

private:
  void behaviorIdle(rpg::Command command);
  void behaviorAggressive(rpg::Command command);
  void behaviorRoaming(rpg::Command command);

private:
  std::string actorType_;
  std::string currentBehavior_;
  int behaviorTimeout_{9999};
  std::unordered_map<std::string, Behaviors> behaviorsList_;

  // Move controller variables
  float speed_walk_ = 0.7f;
  float speed_run_ = 1.50f;
  float speed_dash_ = 3.f;
  float speed_ = 300.f;

  sf::Clock clock;

  mathfu::Vector<float, 2> direction{0.0f, 0.0f};
  mathfu::Vector<float, 2> force{0.0f, 0.0f};
  mathfu::Vector<float, 2> position_{0.0f, 0.0f};
  mathfu::Vector<float, 2> positionOld_{0.0f, 0.0f};

  std::deque<mathfu::Vector<float, 2>> route_{};

  bool isColliding_{false};

  std::deque<rpg::Action> actions_;

  friend class PhysicsBox;
  friend class NavSystem;

  bool drawDebug_{false};
  sf::Clock clock_;
  sf::Time updateTimer{sf::Time::Zero};
};

}
