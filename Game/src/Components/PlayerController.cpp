#include "precompiled.h"
#include "Components/PlayerController.h"
#include "Engine.h"
#include "ECSystem.h"
#include "Globals.h"
#include "Components/PhysicsBox.h"
#include "Components/Sound.h"
#include "Physics.h"
#include <SelbaWard.hpp>
#include <AnimatedSprite.h>
#include "Macroses.h"
#include "CollisionSystem.h"

rpg::PlayerController::PlayerController(int id, int owner, std::string name) : Component(id, owner, name) {

}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::PlayerController::update(sf::Time dt) {
//  IPROF_FUNC;
  Component::update(dt);

  sf::Vector2f pos = owner_->getPosition();

  this->setPosition(pos);

  position_ = {pos.x, pos.y};

  for (auto s : actions_) {
    updateActions(s, dt);
  }

  //Phys positions update
  auto phys_body = dynamic_cast<PhysicsBox *>(MGR->getComponent(owner_->id(), "CollisionBox"));
  auto phys_pos = phys_body->body_->body->GetPosition();
  sf::Vector2f orig, dest, offset;
  dest.x = phys_pos.x * GRID;
  dest.y = phys_pos.y * GRID;
  orig = owner_->getPosition();
  offset = dest - orig;
  owner_->move(offset);

  // Non phys collisions
  auto player_capsule = MGR->getComponent(ownerId_, "Capsule");
  auto coll = COLLSYS->getCollisions(reinterpret_cast<CollisionBox *>(player_capsule));
  if (!coll.empty() && !isColliding_) {
//    printf("Collide!\n");
    isColliding_ = true;
    voice_onhit();
  }
  if (coll.empty()) {
    isColliding_ = false;
  }
}

void rpg::PlayerController::handleEvent(const sf::Event &event) {
  Component::handleEvent(event);

  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Up) {
      actions_.push_back(Action::move_up);
    }
    if (event.key.code == sf::Keyboard::Down) {
      actions_.push_back(Action::move_down);
    }
    if (event.key.code == sf::Keyboard::Left) {
      actions_.push_back(Action::move_left);
    }
    if (event.key.code == sf::Keyboard::Right) {
      actions_.push_back(Action::move_right);
    }

    if (event.key.code == sf::Keyboard::Space) {
      attack();
    }

    if (event.key.code == sf::Keyboard::E) {
      voice_onhit();
    }

    auto last = std::unique(actions_.begin(), actions_.end());
    actions_.erase(last, actions_.end());
  }

  if (event.type == sf::Event::KeyReleased) {
    if (event.key.code == sf::Keyboard::Up) {
      actions_.erase(std::remove(actions_.begin(), actions_.end(), Action::move_up),
                     actions_.end());
    }
    if (event.key.code == sf::Keyboard::Down) {
      actions_.erase(std::remove(actions_.begin(), actions_.end(), Action::move_down),
                     actions_.end());
    }
    if (event.key.code == sf::Keyboard::Left) {
      actions_.erase(std::remove(actions_.begin(), actions_.end(), Action::move_left),
                     actions_.end());
    }
    if (event.key.code == sf::Keyboard::Right) {
      actions_.erase(std::remove(actions_.begin(), actions_.end(), Action::move_right),
                     actions_.end());
    }

  }

  if (event.type == sf::Event::MouseButtonPressed) {

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

      auto start = owner_->getPosition();
      point = mathfu::Vector<float, 2>(sf::Mouse::getPosition(*window_).x, sf::Mouse::getPosition(*window_).y);

      direction = (point - position_);
      direction.Normalize();

      actions_.push_back(Action::walk);
      mathfu::Vector<float, 2> dist = point - position_;
      dist_to_point = dist.Length();

      MGR->getEntity("Pointer")->setPosition(point.x - 12, point.y - 12);
      MGR->getEntity("Pointer")->setActive(true);

    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
      auto route = NAVSYS->findPath(ownerId_, sf::Mouse::getPosition(*window_).x, sf::Mouse::getPosition(*window_).y);
      setRoute(route);
    }

    auto last = std::unique(actions_.begin(), actions_.end());
    actions_.erase(last, actions_.end());
  }

}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

void rpg::PlayerController::updateActions(rpg::Action action, sf::Time dt) {
//  IPROF_FUNC;
  /**
   * Update actor actions every 0.05 seconds
   */

  updateTimer_ += dt;
  if (updateTimer_.asMilliseconds() < 25) {
    return;
  } else {
    updateTimer_ = clock_.restart();
  }

  auto state = dynamic_cast<rpg::ActorState *>( MGR->getComponent(ownerId_, "ActorState"));
  auto anim = dynamic_cast<rpg::AnimatedSprite *>( MGR->getComponent(ownerId_, "AnimatedSprite"));

  /**
 * Update Player animations according to movement and actions
 */

  if (state != nullptr && anim != nullptr) {

    auto cur_beh = to_string(state->getBehavior());
    anim->setAnimationByState(cur_beh);
  }

  /**
 * Update actor look direction based on movement
 */

  /**
   * Update actor look direction based on movement
   */
  auto deltaPos = position_-positionOld_;

  if (abs(deltaPos.x) > abs(deltaPos.y) ){
    // Horisontal movement
    if (deltaPos.x > 0){
      state->setDirection(Direction::right);
    }
    else if (deltaPos.x < 0){
      state->setDirection(Direction::left);
    }
  }

  else if (abs(deltaPos.y) > abs(deltaPos.x) ){
    // Vertical movement
    if (deltaPos.y > 0){
      state->setDirection(Direction::front);
    }
    else if (deltaPos.y < 0){
      state->setDirection(Direction::back);
    }
  }
  if ( position_ == positionOld_ ){
    state->setDirection(Direction::front);
  }
  positionOld_ = position_;

  /**
 * Move actor according to actions in queue
 */

  const auto need_speed = speed_ * speed_walk;

  switch (action) {
  case Action::move_up: {
    state->setBehavior(Behavior::walk);
    keyMove(0.f, -1.f, need_speed);
  }
    break;

  case Action::move_down: {
    state->setBehavior(Behavior::walk);
    keyMove(0.f, 1.f, need_speed);
  }
    break;

  case Action::move_left: {
    state->setBehavior(Behavior::walk);
    keyMove(-1.f, 0.f, need_speed);

  }
    break;

  case Action::move_right: {
    state->setBehavior(Behavior::walk);
    keyMove(1.f, 0.f, need_speed);
  }
    break;

  case Action::route: {
    state->setBehavior(Behavior::walk);
    if (!route_.empty()) {
      MGR->getEntity("Pointer")->setPosition(route_point.x, route_point.y);
      MGR->getEntity("Pointer")->setActive(true);

      auto start = owner_->getPosition();

      direction = (route_point - position_);
      direction.Normalize();

      moveByRout(direction.x, direction.y, speed_);
    } else {

    }
  }
    break;

  default: {
    mouseMove(direction.x, direction.y, speed_);
    state->setBehavior(Behavior::walk);
  }
  }

}

void rpg::PlayerController::mouseMove(float destX, float destY, float speed) {
  auto capsule = dynamic_cast<PhysicsBox *>(MGR->getComponent(owner_->id(), "CollisionBox"));

  float old_dist = dist_to_point;
  float need_speed = 0.f;

  mathfu::Vector<float, 2> dist = point - position_;
  dist_to_point = dist.Length();

  if (dist_to_point > old_dist) {
    //MGR->getEntity("Pointer").setActive(false);
    capsule->setVelocity(0.f, 0.f, 0.f);
    actions_.erase(std::remove(actions_.begin(), actions_.end(), Action::walk),
                   actions_.end());
    return;
  }

  need_speed = speed * speed_walk;
  capsule->setVelocity(destX, destY, need_speed);

}

void rpg::PlayerController::keyMove(float destX, float destY, float speed) {

  auto capsule = dynamic_cast<PhysicsBox *>(MGR->getComponent(owner_->id(), "CollisionBox"));
  capsule->setVelocity(destX, destY, speed);

}

void rpg::PlayerController::moveByRout(float destX, float destY, float speed) {
//  printf("moving\n");

  auto capsule = dynamic_cast<PhysicsBox *>(MGR->getComponent(owner_->id(), "CollisionBox"));

  float old_dist = dist_to_point;
  float need_speed = 0.f;

  mathfu::Vector<float, 2> dist = route_point - position_;
  dist_to_point = dist.Length();

  if (dist_to_point > old_dist) {
    //MGR->getEntity("Pointer").setActive(false);
//    capsule->setVelocity(0.f, 0.f, 0.f);
//    anim_state_.erase(std::remove(anim_state_.begin(), anim_state_.end(), Action::route),
//                      anim_state_.end());
//    return;
  } else if ((dist_to_point <= 5.f) && !route_.empty()) {
    route_point = route_.front();
    route_.pop_front();
    return;
  }

  need_speed = speed * speed_walk;
  capsule->setVelocity(destX, destY, need_speed);

}

void rpg::PlayerController::setRoute(std::deque<mathfu::Vector<float, 2>> path) {

  //printf("Got new route\n");
  std::deque<mathfu::Vector<float, 2>> empty;
  std::swap(route_, empty);
  route_ = path;
  actions_.push_back(Action::route);
  route_point = route_.front();
  route_.pop_front();
}

void rpg::PlayerController::draw(sf::RenderWindow *window) {
  Component::draw(window);

//  if(!route.empty()) {
//    mathfu::Vector<float, 2> point_a, point_b;
//    point_a = route.front();
////  sw::Line line;
//
//    for (int i = 1; i < route.size(); ++i) {
//      point_b = route[i];
//      sw::Line line({point_a.x, point_a.y}, {point_b.x, point_b.y});
//      window_->draw(line);
//
//      point_a = point_b;
//    }
//  }
}

void rpg::PlayerController::attack() {
//  printf("Attack\n");
  dynamic_cast<rpg::Sound *>(MGR->getComponent(ownerId_, "Sound"))->playSound("Punch");
  dynamic_cast<rpg::Sound *>(MGR->getComponent(ownerId_, "Sound"));
  auto anims = dynamic_cast<rpg::AnimatedSprite *>(MGR->getComponent(ownerId_, "Animations"))->getAnimations();

  auto rand_anim = anims[rand() % anims.size()];

  dynamic_cast<rpg::AnimatedSprite *>(MGR->getComponent(ownerId_, "Animations"))->setAnimation(rand_anim);

}

void rpg::PlayerController::voice_onhit() {

  std::map<int, std::string> voices{{0, "Ahh"}, {1, "Ohh"}, {2, "Yay"}};
  auto r = rand() % 3;
  auto voice = voices.at(r);
  dynamic_cast<rpg::Sound *>(MGR->getComponent(ownerId_, "Sound"))->playSound(voice);

//  printf("Voice on hit: %s\n", voice.c_str());
}

//void rpg::Controller::executeCommand(const rpg::Command &command) {
//  printf("Got command %d\n", command.id);
//}
