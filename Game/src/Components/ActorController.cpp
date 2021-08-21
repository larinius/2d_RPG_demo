#include "ActorController.h"
#include "ActorState.h"
#include "Entity.h"
#include "Engine.h"
#include "AnimatedSprite.h"
#include <nlohmann/json.hpp>
#include <CollisionBox.h>
#include <SelbaWard/Line.hpp>
#include "PhysicsBox.h"
#include "Enums.h"

rpg::ActorController::ActorController(int id, int owner, std::string name) : Component(id, owner, name) {

}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::ActorController::draw(sf::RenderWindow *window) {
  Component::draw(window);

  if (drawDebug_) {
    if (!route_.empty()) {
      mathfu::Vector<float, 2> point_a, point_b;
      point_a = position_;
//  sw::Line line;

      for (int i = 0; i < route_.size(); ++i) {
        point_b = route_[i];
        sw::Line line({point_a.x, point_a.y}, {point_b.x, point_b.y});
        window_->draw(line);

        point_a = point_b;
      }
    }
  }

}

void rpg::ActorController::update(sf::Time dt) {
  IPROF_FUNC;
  Component::update(dt);


  // Random behavior changing by fined timeouts
  behaviorTimeout_ -= dt.asMilliseconds();
  if (behaviorTimeout_ <= 0) {
    switchBehavior(Behavior::idle);
  }

  //=================================
  sf::Vector2f pos = owner_->getPosition();
  this->setPosition(pos);
  position_ = {pos.x, pos.y};

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
//  auto coll = COLLSYS->getCollisions(reinterpret_cast<CollisionBox *>(player_capsule));
//  if (!coll.empty() && !isColliding_) {
//    printf("Collide!\n");
//    isColliding_ = true;
//    voice_onhit();
//  }
//  if (coll.empty()) {
//    isColliding_ = false;
//  }

  /**
   * Execute actions from queue.
   */
  for (auto s : actions_) {
      updateActions(s, dt);
  }

}



void rpg::ActorController::handleEvent(const sf::Event &event) {
  Component::handleEvent(event);
}

void rpg::ActorController::onCommand(rpg::Command command) {
//  printf("Got command command: %s - %s\n", owner_->name().c_str(), to_string(command.action).c_str());

  if (command.action == Action::set_behavior) {
    currentBehavior_ = command.parameter;

    switch (command.behavior) {

    case Behavior::idle:behaviorIdle(command);
      break;
    case Behavior::roaming:behaviorRoaming(command);
      break;
    case Behavior::aggressive:behaviorAggressive(command);
      break;
    }
  }
}

void rpg::ActorController::onActivate() {
  Component::onActivate();
  loadBehaviors(actorType_);
}

void rpg::ActorController::onDestroy() {
  Component::onDestroy();
}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

///////////////////////////////////////////////
// Movement methods
///////////////////////////////////////////////
void rpg::ActorController::moveToPoint(float posX, float posY) {

  auto capsule = dynamic_cast<PhysicsBox *>(MGR->getComponent(owner_->id(), "CollisionBox"));

  //float old_dist = dist_to_point_;
  float need_speed = 0.f;

  mathfu::Vector<float, 2> point = {posX, posY};
  mathfu::Vector<float, 2> dist = point - position_;

  if (dist.Length() > 16.f) {
    //MGR->getEntity("Pointer").setActive(false);
    capsule->setVelocity(0.f, 0.f, 0.f);
    actions_.erase(std::remove(actions_.begin(), actions_.end(), rpg::Action::walk),
                   actions_.end());
    return;
  }

  need_speed = speed_ * speed_walk_;
  capsule->setVelocity(posX, posY, need_speed);

}

void rpg::ActorController::setRoute(std::deque<mathfu::Vector<float, 2>> route) {
  IPROF_FUNC;
//  printf("%s: Got new route!\n", owner_->name().c_str());
  std::deque<mathfu::Vector<float, 2>> empty;
  std::swap(route_, empty);
  route_ = route;
  actions_.erase(std::remove(actions_.begin(), actions_.end(), rpg::Action::route), actions_.end());
  actions_.push_back(Action::route);
}

void rpg::ActorController::moveByRout(std::deque<mathfu::Vector<float, 2>>, float speed) {
  IPROF_FUNC;
//  printf("Route: %d\n", route_.size());

  auto capsule = dynamic_cast<PhysicsBox *>(MGR->getComponent(owner_->id(), "CollisionBox"));

  /**
   * Stop if close to route point, remove this point from route, we dont need it any more.
   */
  auto dist = route_.front() - position_;
//  if (dist.Length() <= 10.f && route_.size() == 1) {
//    route_.pop_front();
//    capsule->setVelocity(0, 0, 0);
//    return;
//  }
  if (dist.Length() <= 10.f) {
    route_.pop_front();
    capsule->setVelocity(0, 0, 0);
    return;
  }

  /**
   * Take point from top of deque, normalize it, and apply force with normalised vector.
   */
  auto point = (route_.front() - position_).Normalized();

  auto cur_speed = capsule->body_->body->GetLinearVelocity();

  if (cur_speed.Length() <=  1.0f) {
    capsule->setVelocity(point.x, point.y, speed * speed_walk_);
  }
}

///////////////////////////////////////////////
// Behaviors methods
///////////////////////////////////////////////

void rpg::ActorController::loadBehaviors(std::string path) {
//  printf("%s, path: %s\n", __FUNCTION_NAME__, path.c_str());
  if (actorType_.empty()) {
    return;
  }
  using json = nlohmann::json;
  std::string key = actorType_;
  auto file_path = "Assets/Sprites/" + actorType_ + "/behaviors.json";
  assert(helper::isPathValid(file_path));
  std::ifstream ifs(file_path);
  json j = json::parse(ifs);

  for (auto &el : j[actorType_]["behaviors"].items()) {
    Behaviors b;

    b.name = el.value()["name"];
    b.timeout = el.value()["timeout"];
    b.repeat = el.value()["repeat"];
    b.radius = el.value()["radius"];
    b.priority = el.value()["priority"];

    behaviorsList_.insert({b.name, b});
  }
}

void rpg::ActorController::switchBehavior(rpg::Behavior behavior) {

  if ((rand() % 2) == 1) {
    COMMAND->dispatchCommand(ownerId_, Action::set_behavior, Behavior::roaming);
  } else {
    COMMAND->dispatchCommand(ownerId_, Action::set_behavior, Behavior::idle);
  }

}

void rpg::ActorController::behaviorIdle(rpg::Command command) {
//  printf("%s: Im %s!\n", owner_->name().c_str(), to_string(command.behavior).c_str());

  auto state = dynamic_cast<rpg::ActorState *>( MGR->getComponent(ownerId_, "ActorState"));
  auto anim = dynamic_cast<rpg::AnimatedSprite *>( MGR->getComponent(ownerId_, "AnimatedSprite"));

  if (state != nullptr && anim != nullptr) {
    state->setAction(command.action);
    state->setBehavior(command.behavior);
    currentBehavior_ = behaviorsList_.at(to_string(command.behavior)).name;
    behaviorTimeout_ = behaviorsList_.at(to_string(command.behavior)).timeout;

//    state->setDirection(Direction::left);
    anim->setAnimationByState(to_string(command.behavior));
  }

}

void rpg::ActorController::behaviorAggressive(rpg::Command command) {
//  printf("Im %s!\n", to_string(command.behavior).c_str());

  auto state = dynamic_cast<rpg::ActorState *>( MGR->getComponent(ownerId_, "ActorState"));
  auto anim = dynamic_cast<rpg::AnimatedSprite *>( MGR->getComponent(ownerId_, "AnimatedSprite"));

  if (state != nullptr && anim != nullptr) {
    state->setAction(command.action);
    state->setBehavior(command.behavior);
    currentBehavior_ = behaviorsList_.at(to_string(command.behavior)).name;
    behaviorTimeout_ = behaviorsList_.at(to_string(command.behavior)).timeout;

//    state->setDirection(Direction::left);
    anim->setAnimationByState(to_string(command.behavior));
  }
}

void rpg::ActorController::behaviorRoaming(rpg::Command command) {
  IPROF_FUNC;
//  printf("Im %s!\n", to_string(command.behavior).c_str());

  int radius = behaviorsList_.at(to_string(command.behavior)).radius * GRID;

  auto rnd = NAVSYS->getRandomNavigable(position_.x, position_.y, radius);

//  auto route = NAVSYS->findPath(ownerId_, rnd.x, rnd.y);
  std::deque<mathfu::Vector<float, 2>> route;

  route.push_back(rnd);
  route.push_back(rnd);
//  route.push_back(position_);//start

  if (route.empty()) {
    return;
  }

  setRoute(route);
  auto state = dynamic_cast<rpg::ActorState *>( MGR->getComponent(ownerId_, "ActorState"));
  auto anim = dynamic_cast<rpg::AnimatedSprite *>( MGR->getComponent(ownerId_, "AnimatedSprite"));

  if (state != nullptr && anim != nullptr) {
    state->setAction(command.action);
    state->setBehavior(command.behavior);
    currentBehavior_ = behaviorsList_.at(to_string(command.behavior)).name;
    behaviorTimeout_ = behaviorsList_.at(to_string(command.behavior)).timeout;

//    state->setDirection(Direction::left);
    anim->setAnimationByState(to_string(command.behavior));

  }
}

void rpg::ActorController::updateActions(rpg::Action action, sf::Time dt) {
  IPROF_FUNC;

  /**
   * Update actor actions every 0.1 seconds
   */

  updateTimer += dt;
  if(updateTimer.asMilliseconds() < 100){
    return;
  }
  else{
    updateTimer = clock_.restart();
  }

  /**
   * Update actor look direction based on movement
   */
  auto state = dynamic_cast<rpg::ActorState *>( MGR->getComponent(ownerId_, "ActorState"));
  if(positionOld_.x <  position_.x){
    state->setDirection(Direction::right);
    positionOld_ = position_;
  }
  else{
    state->setDirection(Direction::left);
    positionOld_ = position_;
  }

  /**
   * Move actor according to actions in queue
   */

  float need_speed = 0.f;
  need_speed = speed_ * speed_walk_;

  auto capsule = dynamic_cast<PhysicsBox *>(MGR->getComponent(owner_->id(), "CollisionBox"));
  switch (action) {
  case Action::move_up: {

    keyMove(0.f, -1.f, need_speed);
  }
    break;

  case Action::move_down: {

    keyMove(0.f, 1.f, need_speed);
  }
    break;

  case Action::move_left: {

    keyMove(-1.f, 0.f, need_speed);
  }
    break;

  case Action::move_right: {

    keyMove(1.f, 0.f, need_speed);
  }
    break;

  case Action::route: {
    if (!route_.empty()) {
      moveByRout(route_, speed_);
    } else {

    }
  }
    break;
  }
}
