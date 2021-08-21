#include "precompiled.h"
#include "Enums.h"

std::string rpg::to_string(rpg::Action action) {

  std::string result;

  switch (action) {

  case Action::idle:result = "idle";
    break;
  case Action::set_behavior:result = "behavior";
    break;
  case Action::move_left:result = "move_left";
    break;
  case Action::move:result = "move";
    break;
  case Action::attack:result = "attack";
    break;
  case Action::use:result = "use";
    break;
  }

  return result;
}

std::string rpg::to_string(rpg::Behavior behavior) {
  std::string result;

  switch (behavior) {

  case Behavior::idle: result = "idle";
    break;
  case Behavior::roaming: result = "roaming";
    break;
  case Behavior::aggressive: result = "aggressive";
    break;
  case Behavior::walk: result = "walk";
    break;
  case Behavior::run: result = "run";
    break;
  }

  return result;
}

std::string rpg::to_string(rpg::Direction direction) {
  std::string result;

  switch (direction) {

  case Direction::back: result = "back";
    break;
  case Direction::front: result = "front";
    break;
  case Direction::left: result = "left";
    break;
  case Direction::right: result = "right";
    break;
  }

  return result;
}
