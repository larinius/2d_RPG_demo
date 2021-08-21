#include "CollisionSystem.h"
#include "CollisionBox.h"

rpg::CollisionSystem::CollisionSystem(std::string name, int id) : Object(name, id) {

}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::CollisionSystem::draw(sf::RenderWindow *window) {

}

void rpg::CollisionSystem::update(sf::Time dt) {

  if (!bodies_.empty()) {
    auto i = 0;
  }
}

void rpg::CollisionSystem::handleEvent(const sf::Event &event) {

}

void rpg::CollisionSystem::onActivate() {

}

void rpg::CollisionSystem::onDestroy() {

}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

void rpg::CollisionSystem::addBody(rpg::CollisionBox *body) {
  bodies_.push_back(body);
}

void rpg::CollisionSystem::removeBody(rpg::CollisionBox *body) {

}

bool rpg::CollisionSystem::isCollide(const rpg::CollisionBox &bodyA, const rpg::CollisionBox &bodyB) {
  IPROF_FUNC;
  if(bodyA.id() == bodyB.id()){
    return false;
  }

  auto dist = std::abs((bodyA.getPos() - bodyB.getPos()).Length());
  auto sumSize = (bodyA.getSize() + bodyB.getSize())/2.f;

  return dist <= sumSize ? true : false;
}

std::vector<rpg::CollisionBox *> rpg::CollisionSystem::getCollisions(rpg::CollisionBox *capsule) {
  IPROF_FUNC;
  std::vector<rpg::CollisionBox *> result{};

  if(!bodies_.empty()) {

    std::copy_if(bodies_.begin(), bodies_.end(), back_inserter(result),
                 [&](rpg::CollisionBox *b) { return isCollide(*capsule, *b); });
  }
    return result;

}
