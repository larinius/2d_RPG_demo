#include "CommandQueue.h"
#include "ECSystem.h"
#include "Engine.h"

rpg::CommandQueue::CommandQueue(std::string name, int id) : Object(name, id) {

}

void rpg::CommandQueue::update(sf::Time dt) {
  while(!commands_.empty()){
    auto command = std::move(commands_.front());
    commands_.pop_front();
    MGR->getEntity(command.listener_id)->onCommand(command);
  }
}

int rpg::CommandQueue::dispatchCommand(int listener_id, rpg::Action action, mathfu::Vector<float, 2> pos, int target_id) {

  Command command;
  int id = ID;
  command.id = id;
  command.action = action;
  command.pos = pos;
  command.target_id = target_id;
  command.listener_id = listener_id;

  commands_.push_back(command);

  return id;
}

std::vector<rpg::Command> rpg::CommandQueue::getCommands() {

  std::vector<Command> result;
  for(const auto & comm : commands_)
    result.push_back(comm);

  return result;
}

void rpg::CommandQueue::subscribe(int listener_id) {
  auto result = std::find_if(listeners_.begin(), listeners_.end(),
                             [&](const int &i) { return (i == listener_id); });
  if (result != listeners_.end()) {
    return;
  }

  listeners_.push_back(listener_id);
}

//int rpg::CommandQueue::dispatchCommand(int listener_id, rpg::Action action, std::string name) {
//  Command command;
//  int id = ID;
//
//  command.id = id;
//  command.action = action;
//  command.listener_id = listener_id;
//  command.parameter = name;
//  commands_.push_back(command);
//
//  return id;
//}

int rpg::CommandQueue::dispatchCommand(int listener_id, rpg::Action action, rpg::Behavior behavior) {
  Command command;
  int id = ID;

  command.id = id;
  command.action = action;
  command.listener_id = listener_id;
  command.behavior = behavior;
  commands_.push_back(command);

  return id;
}
