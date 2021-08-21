#include "precompiled.h"
#include "State/State.h"

void rpg::State::setActive(bool active) {
  if(active) {
    onActivate();
  }
  else{
    active_ = false;
  }
}
