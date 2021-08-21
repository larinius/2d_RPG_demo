#pragma once
#include "precompiled.h"
#include "Components/Component.h"
#include "CommandQueue.h"
#include "ActorController.h"

namespace rpg {

class Entity : public sf::Transformable, public rpg::CommandListener {

public:
  explicit Entity(int id, std::string name);
  Entity(const Entity &) = delete;
  Entity &operator=(const Entity &) = delete;
  ~Entity() {onDestroy();}

  // Common API
  void draw(sf::RenderWindow *window);
  virtual void update(sf::Time dt);
  virtual void handleEvent(const sf::Event &event);

  // Event API
  void setActive(bool);

  // API for <Component>
  void addComponent(int id) { components_.push_back(id); }
  void removeComponent(int id);
  std::vector<int> components() { return components_; }
  std::vector<int> componentsByTag(int id);
//  Component& getComponent(std::string name);

  // Attachments API <Entity>
  void addChild(int id) { attachments_.push_back(id); }
  void removeChild(int id);
  std::vector<int> attachments() { return attachments_; };
  std::vector<int> get_attachments_by_tag(int id);

  // Getters and setters
  int id() const { return id_; }
  std::string name() { return name_; }
  bool isActive() const { return active_; }
  void setSize(int size){size_ = size;}
  int getSize(){return size_;}

  void setTag(int id) { tags_.push_back(id); }
  void removeTag(int id);
  std::vector<int> getTags();

  void onCommand(Command command) override;

protected:
  void onActivate();
  void onDestroy();

protected:
  const int id_;
  int ownerId_{0};
  int parentId_{0};
  const std::string name_;

  std::vector<int> attachments_;  //ids of attached Entities
  std::vector<int> components_;   //ids of attached Components
  std::vector<int> tags_;         //ids of asigned Tags

  //Init me after spawn
  bool active_;
  Entity *owner_{nullptr};
  sf::RenderWindow *window_;

  int size_{static_cast<int>(GRID)};
};

}
