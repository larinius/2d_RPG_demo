#pragma once
#include "precompiled.h"
#include "Tag.h"
//#include "Engine.h"

namespace rpg {

class Engine;

enum class CType {
  image,
  boneAnimation,
  particle,
  sound,
  collision,
  capsule,
  transform,
  movement,
  guiWidget,
  controller,
  walls,
  animatedSprite,
  navigation,
  state,
  commandlistener
};

class Entity;
//class Engine;

class Component {

public:
  explicit Component(int id, int owner, std::string name);
  Component(const Component &) = delete;
  Component &operator=(const Component &) = delete;
  ~Component() = default;

  // Common API
  virtual void draw(sf::RenderWindow *window);
  virtual void update(sf::Time dt);
  virtual void handleEvent(const sf::Event &event);
  virtual void setActive(bool active);

  // Class members API
  void addChild(int id) { attachments_.push_back(id); }
  void removeChild(int id);

  // Getters and setters
  int id() const { return id_; }
  std::string name() { return name_; }
  bool isActive() { return active_; }
  int owner() { return ownerId_; }
  std::vector<int> getAll() { return attachments_; }

  void setTag(int id) { tags_.push_back(id); }
  void removeTag(int id);
  std::vector<int> getTags();
  mathfu::Vector<float, 2> getPos() const { return position_; }
  float getSize() const {return size_;}

protected:
  virtual void onActivate();      //Call to setup all data
  virtual void onDestroy();       //Call to clan-up children

protected:
  const int id_;
  const int ownerId_{0};         //id of Entity which owns this oject
  const int parentId_{0};        //id of other Component if it atached
  const std::string name_;

  std::vector<int> attachments_;  //ids of attached components
  std::vector<int> tags_;         //ids of assigned tags
  mathfu::Vector<float, 2> position_{0.0f, 0.0f};
  float size_{GRID};

  //Init me
  bool active_{false};
  Entity *owner_{nullptr};
  Component *parent_{nullptr};
  sf::RenderWindow *window_;
};

//struct less_than_y {
//  inline bool operator()(const std::unique_ptr<rpg::Component> &a, const std::unique_ptr<rpg::Component> &b) {
//    return (a->getPos().y < b->getPos().y);
//  }

//  inline bool operator()(const unique_ptr<Worker>& a, const unique_ptr<Worker>& b)
//  {
//    return a->getName() < b->getName();
//  }

//};

}
