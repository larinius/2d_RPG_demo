#include "precompiled.h"
#include "Physics.h"
#include "SFMLDebugDraw.h"
#include "Helpers.h"
#include "Engine.h"
#include "Globals.h"
#include "Macroses.h"

rpg::Physics::Physics(std::string name, int id) : Object(name, id) {
//  printf("%s\n", __FUNCTION_NAME__);
}

void rpg::Physics::onActivate() {
//  printf("%s\n", __FUNCTION_NAME__);
  window_ = RWIND;
  world_ = std::make_unique<b2World>(gravity_);

  if (drawDebug_) {
    debugDraw_ = std::make_unique<SFMLDebugDraw>(*window_);
    world_->SetDebugDraw(debugDraw_.get());
    /* Set initial flags for what to draw */
//    debugDraw_->SetFlags(b2Draw::e_aabbBit); //Only draw shapes
//  debugDraw_->SetFlags(b2Draw::e_centerOfMassBit);
    debugDraw_->SetFlags(b2Draw::e_shapeBit); //Only draw shapes
  }
}

void rpg::Physics::update(sf::Time dt) {
  IPROF_FUNC;
  if (active_) {
    world_->Step(timeStep, velocityIterations_, positionIterations_);
  }
}

rpg::Body *rpg::Physics::createAABB(std::string name, float posX, float posY, float size) {
//  printf("%s\n", __FUNCTION_NAME__);
  auto b = std::make_shared<rpg::Body>();
//  rpg::Body b;
  int id = ID;
  b->id = id;
  b->name = (name + "_" + std::to_string((objects_.size() + 1)));
  b->is_active = true;
//  b->bodyDef.position.x = posX / GRID;
//  b->bodyDef.position.y = posY / GRID;
  b->bodyDef.position.x = posX / GRID;
  b->bodyDef.position.y = posY / GRID;
  b->bodyDef.type = b2_dynamicBody;
  b->bodyDef.allowSleep = true;
  b->bodyDef.fixedRotation = true;

  b->body = world_->CreateBody(&b->bodyDef);
//  b->boxShape.SetAsBox(size * SCALE, size * SCALE, b2Vec2(0.f, 0.f), 0);
  b->circleShape.m_p.Set(0, 0); //position, relative to body position
  b->circleShape.m_radius = 0.5; //radius
  b->circleShape.m_p.Set(0, 0); //position, relative to body position
//  b->body->CreateFixture(&b->boxShape, 1.f); //Old Box shape
  b->body->CreateFixture(&b->circleShape, 1.f); // new round body


  objects_.push_back(b);

  return b.get();
}

rpg::Body *rpg::Physics::createStatic(std::string name, float posX, float posY, float sizeX, float sizeY) {
//  printf("%s\n", __FUNCTION_NAME__);
  auto static_body = std::find_if(objects_.begin(), objects_.end(),
                                  [&](std::shared_ptr<rpg::Body> const &i) { return i.get()->name == name; });

  if (static_body != objects_.end()) {
//    printf("Body found %s\n", static_body->get()->name.c_str());
//    //=====================================
    b2PolygonShape polygonShape;
    b2FixtureDef fixtureDef;

    auto x = posX / GRID;
    auto y = posY / GRID;

//    polygonShape.SetAsBox(sizeX * SCALE, sizeY * SCALE, b2Vec2(x, y), 0);



    static_body->get()->boxShape.SetAsBox(sizeX * SCALE, sizeY * SCALE, b2Vec2(x, y), 0);

//    fixtureDef.shape = &polygonShape;
    fixtureDef.shape = &static_body->get()->boxShape;
    fixtureDef.density = 1;

    static_body->get()->body->CreateFixture(&fixtureDef);

    return static_body->get();
    //================->====================
  } else {
    printf("Body not found create new %s\n", name.c_str());

//  printf("WAL: %s, %f, %f, %f, %f\n", name.c_str(), posX, posY, sizeX, sizeY);

    auto b = std::make_shared<rpg::Body>();

    auto x = posX / GRID;
    auto y = posY / GRID;

    int id = ID;
    b->id = id;
    b->name = (name);
    b->is_active = true;
    b->bodyDef.position.x = 0;
    b->bodyDef.position.y = 0;
    b->bodyDef.type = b2_staticBody;
    b->bodyDef.allowSleep = true;
    b->bodyDef.fixedRotation = true;

    b->body = world_->CreateBody(&b->bodyDef);
    b->boxShape.SetAsBox(sizeX * SCALE, sizeY * SCALE, b2Vec2(0, 0), 0);
    b->body->CreateFixture(&b->boxShape, 1.f);

    objects_.push_back(b);

    return b.get();
  }

}

void rpg::Physics::draw(sf::RenderWindow *window) {
  IPROF_FUNC;
  if (drawDebug_) {
    world_->DebugDraw();
  }
}

void rpg::Physics::handleEvent(const sf::Event &event) {

}

void rpg::Physics::onDestroy() {
  clear();
}

void rpg::Physics::clear() {

//  auto old_world = std::move(world_);
//  auto new_world_ = std::make_unique<b2World>(gravity_);
//  old_world.reset();
//  world_ = std::move(new_world_);

//  world_.reset();
//  world_ = std::make_unique<b2World>(gravity_);

  auto b = world_->GetBodyList();
  while (b != nullptr) {
    auto d = b;
    b = b->GetNext();
    world_->DestroyBody(d);
  }

//  for (auto &del : objects_) {
//    world_->DestroyBody(del->body);
//  }

  objects_.clear();

  auto c = world_->GetBodyCount();

  printf("%s, %d bodies\n", __FUNCTION_NAME__, c);
}
