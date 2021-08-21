#pragma once
#include "precompiled.h"

namespace rpg {
/**
 * Base class for all objects.
 */
class Object {
public:
  Object(std::string name, int id) : name_(name), id_(id){};
  Object(Object const &) = delete;
  Object &operator=(Object const &) = delete;
  ~Object() = default;

  // Interface methods
  virtual void draw(sf::RenderWindow *window) = 0;
  virtual void update(sf::Time dt) = 0;
  virtual void handleEvent(const sf::Event &event) = 0;

  virtual void setActive(bool active){if(!active_ && active){onActivate();} active_ = true;}
  bool isActive() {return active_;}
  virtual std::string name() {return name_;}
  virtual int id() {return id_;}

protected:
  //interface methods
  virtual void onActivate() = 0;
  virtual void onDestroy() = 0;

  bool active_{false};
  const std::string name_;
  const int id_;

};
}
