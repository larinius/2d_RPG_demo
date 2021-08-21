#pragma once
#include "precompiled.h"
#include "Object.h"

namespace rpg {

std::string to_string(Action action);


class CommandListener{
public:
  CommandListener() = default;
  virtual void onCommand(Command command) = 0;

protected:
  std::deque<rpg::Command> commands_;
};

class CommandQueue : public Object {

public:
  CommandQueue(std::string name, int id);
  void draw(sf::RenderWindow *window) override {};
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override {};

  int dispatchCommand(int listener_id, Action action, mathfu::Vector<float, 2> pos= {0.f, 0.f}, int target_id= 0);
//  int dispatchCommand(int listener_id, Action action, std::string name);
  int dispatchCommand(int listener_id, Action action, Behavior behavior);
  std::vector<rpg::Command> getCommands();
  void subscribe(int listener_id);

private:
  void onActivate() override {};
  void onDestroy() override {};

private:
  std::deque<rpg::Command> commands_;
  std::deque<int> listeners_;
};
}
