#pragma once

namespace rpg {

/**
 * Behavior - complex tasks with multiple actions
 */
enum class Behavior {
  idle,
  roaming,
  aggressive,
  walk,
  run
};

/**
 * Action - simple one step task
 */
enum class Action {
  idle,
  move,
  attack,
  use,
  jump,
  walk,
  run,
  move_left,
  move_right,
  move_up,
  move_down,
  set_behavior,
  route
};

/**
 * Direction - used to set actors rotation on screen
 */
enum class Direction{
  back,
  front,
  left,
  right
};

std::string to_string(rpg::Action action);

std::string to_string(rpg::Behavior behavior);

std::string to_string(rpg::Direction direction);

}
