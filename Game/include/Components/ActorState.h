#pragma once
#include "precompiled.h"
#include "Component.h"
//#include "CommandQueue.h"
//#include "AI.h"

namespace rpg {

class ActorState : public Component {
public:
  ActorState(int id, int owner, std::string name);
  ~ActorState() { onDestroy(); }

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt)  override;
  void handleEvent(const sf::Event &event)  override;

private:
  void onActivate()  override;
  void onDestroy()  override;

public:
  void setBehavior(rpg::Behavior behavior);
  void setAction(rpg::Action action);
  void setDirection(rpg::Direction direction);

  rpg::Behavior getBehavior() { return behavior_;};
  rpg::Action getAction() {return action_;};
  rpg::Direction getDirection() {return direction_;}
  std::string getAnimation() {return animation_;};

private:
  rpg::Behavior behavior_;
  rpg::Action action_;
  rpg::Direction direction_;
  std::string animation_;


  friend class AnimatedSprite;
};
}
