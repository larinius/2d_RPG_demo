#pragma once
#include "precompiled.h"
#include "Component.h"
#include "CommandQueue.h"
#include "Entity.h"
#include "PhysicsBox.h"
#include "NavSystem.h"

namespace rpg {

class PlayerController : public Component, public sf::Transformable {
public:
  PlayerController(int id, int owner, std::string name);

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

public:
  void updateActions(rpg::Action action, sf::Time dt);
  void setRoute(std::deque<mathfu::Vector<float, 2>> path);

  void mouseMove(float destX, float destY, float speed);
  void keyMove(float destX, float destY, float speed);
  void moveByRout(float destX, float destY, float speed);
  void moveToPoint(float destX, float destY, float speed);

  void attack();
  void voice_onhit();

private:
  float speed_walk = 2.0f;
  float speed_run = 1.50f;
  float speed_dash = 3.f;
  float dist_to_point = 0.f;
  float dist_to_point_route = 9999.f;

  float speed_ = 100.f;

  sf::Clock clock;

  mathfu::Vector<float, 2> point{0.0f, 0.0f};
  mathfu::Vector<float, 2> direction{0.0f, 0.0f};
  mathfu::Vector<float, 2> force{0.0f, 0.0f};
  mathfu::Vector<float, 2> position_{0.0f, 0.0f};
  mathfu::Vector<float, 2> positionOld_{0.0f, 0.0f};
  mathfu::Vector<float, 2> route_point{0.0f, 0.0f};

  std::deque<mathfu::Vector<float, 2>> route_{};
  bool drawDebug{true};

  bool isColliding_{false};

  std::deque<rpg::Action> actions_;

  sf::Clock clock_;
  sf::Time updateTimer_{sf::Time::Zero};

  friend class PhysicsBox;
  friend class NavSystem;
};
}
