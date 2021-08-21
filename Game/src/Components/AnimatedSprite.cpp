#include "precompiled.h"
#include "Components/AnimatedSprite.h"
#include "Entity.h"
#include "Engine.h"
#include <nlohmann/json.hpp>

rpg::AnimatedSprite::AnimatedSprite(int id, int owner, std::string name) : Component(id, owner, name) {
  frameTime = sf::seconds(0.1f);
}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::AnimatedSprite::draw(sf::RenderWindow *window) {

  if (active_) {
    Component::draw(window);

    sf::RenderStates states;

    auto transform = owner_->getTransform();

    transform.translate(offset_);

//    states.transform *= owner_->getTransform();

    states.transform *= transform;

    states.texture = &currentAnim_->texture;
    window_->draw(currentAnim_->vertices, 4, sf::Quads, states);

  }
}

void rpg::AnimatedSprite::update(sf::Time dt) {

  setAnimationByState(animState_);

  /**
   * Update animation frames
   */

//  Component::update(dt);
  auto pos = owner_->getPosition();
  this->setPosition(pos);
  position_ = {pos.x, pos.y};

  currentAnim_->sprite.setPosition(pos);
  // if not paused and we have a valid animation
  if (!currentAnim_->isPaused && !currentAnim_->frameSet.empty()) {
    // add delta time
    currentTime += dt;

    // if current time is bigger then the frame time advance one frame
    if (currentTime >= frameTime) {
      // reset time, but keep the remainder
      currentTime = sf::microseconds(currentTime.asMicroseconds() % frameTime.asMicroseconds());

      // get next Frame index
      if (currentFrame + 1 < currentAnim_->frameSet.size())
        currentFrame++;
      else {
        // animation has ended
        if (!currentAnim_->isLooped) {
          currentAnim_->isPaused = true;
        } else {
          currentFrame = 0; // reset to start
        }

      }

      // set the current frame, not reseting the time
      setFrame(currentFrame, false);
    }
  }

}

void rpg::AnimatedSprite::onActivate() {
//  printf("%s\n", __FUNCSIG__);

  Component::onActivate();

  if (animations_.empty()) {
//    printf("path_ in: %s\n", this->owner_->name().c_str());
    assert(helper::isPathValid(path_));
    loadAnimations(path_);
  }

  if (current_anim_name_.empty()) {
    current_anim_name_ = animations_.begin()->first;
  }
  setAnimation(current_anim_name_);

  setOffsets();
}

void rpg::AnimatedSprite::onDestroy() {
  Component::onDestroy();
}

///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

void rpg::AnimatedSprite::setFrame(int frameId, bool resetTime) {
//  printf("%s, %s\n", owner_->name().c_str(), __FUNCSIG__);

  if (!currentAnim_->frameSet.empty()) {
    auto frame = currentAnim_->frameSet.at(frameId);
    //calculate new vertex positions and texture coordiantes
    sf::IntRect rect(frame.x, frame.y, frame.w, frame.h);

    currentAnim_->vertices[0].position = sf::Vector2f(0.f, 0.f);
    currentAnim_->vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.height));
    currentAnim_->vertices[2].position =
        sf::Vector2f(static_cast<float>(rect.width), static_cast<float>(rect.height));
    currentAnim_->vertices[3].position = sf::Vector2f(static_cast<float>(rect.width), 0.f);

    auto left = static_cast<float>(rect.left) + 0.0001f;
    auto right = left + static_cast<float>(rect.width);
    auto top = static_cast<float>(rect.top);
    auto bottom = top + static_cast<float>(rect.height);

    currentAnim_->vertices[0].texCoords = sf::Vector2f(left, top);
    currentAnim_->vertices[1].texCoords = sf::Vector2f(left, bottom);
    currentAnim_->vertices[2].texCoords = sf::Vector2f(right, bottom);
    currentAnim_->vertices[3].texCoords = sf::Vector2f(right, top);
  }

  if (resetTime)
    currentTime = sf::Time::Zero;
}

int rpg::AnimatedSprite::loadAnimations(std::string path) {
//  printf("%s\n", __FUNCSIG__);

  auto full_path = path + "animations.json";

  assert(helper::isPathValid(full_path));
  using json = nlohmann::json;
  std::ifstream ifs(full_path);
  json j = json::parse(ifs);

//  std::string key = "frames";

  for (auto &el : j.items()) {

    std::string anim_name = el.value()["Name"];
    std::string json = el.value()["Animation"];
    std::string json_path = path_ + json;
    assert(helper::isPathValid(json_path));
    auto anim = std::make_unique<Animation>();
    anim->isLooped = el.value()["Looped"];
    anim->state = el.value()["State"];
    anim->name = el.value()["Name"];
    anim->frameSet = loadFrameSet(json_path);

    std::string texturePath = path_ + anim->frameSet.at(0).atlasFile;
    anim->texture.loadFromFile(texturePath);
    anim->texture.setSmooth(true);
    anim->sprite.setTexture(anim->texture);

    animations_.insert({anim_name, std::move(anim)});
  }

  return 0;
}

void rpg::AnimatedSprite::setPath(std::string path) {
  assert(rpg::helper::isPathValid(path));
  path_ = std::move(path);

}

void rpg::AnimatedSprite::setAnimation(std::string name) {
  current_anim_name_ = name;
  currentAnim_ = animations_.at(current_anim_name_).get();
}

void rpg::AnimatedSprite::setAnimationByState(std::string state) {
//  printf("%s\n", __FUNCSIG__);


  if (state.empty()) {
//    printf("State empty\n");
    return;
  }

  animState_ = state;

  auto actorState = dynamic_cast<rpg::ActorState *>( MGR->getComponent(ownerId_, "ActorState"));
  auto direction = to_string(actorState->getDirection());
  auto anim_name = state + "_" + direction;



  printf("Set animation for %s, %s\n", owner_->name().c_str(), anim_name.c_str());

  if(current_anim_name_ != anim_name) {
    current_anim_name_ = anim_name;

    auto result = std::find_if(animations_.begin(), animations_.end(),
                               [&](const auto &i) { return i.second->state == anim_name; });
    if (result != animations_.end()) {
      setAnimation(result->second->name);
    }
  }
}

std::vector<std::string> rpg::AnimatedSprite::getAnimations() {
  std::vector<std::string> anims;

  for (const auto &a : animations_) {
    anims.push_back(a.first);
  }

  return anims;
}

rpg::FrameSet rpg::loadFrameSet(std::string path) {
//  printf("%s\n", __FUNCSIG__);

//  auto frameset = std::make_unique<rpg::helper::FrameSet>();
  FrameSet frameset;

  rpg::AnimationFrame frame;

  using json = nlohmann::json;
  std::string key = "frames";
  std::ifstream ifs(path);
  json j = json::parse(ifs);
//  auto t = j["frames"]["frame_0"]["frame"]["x"];

  int duration = 1000 / 24 / j["frames"].size(); //milliseconds
  int frameId = 0;
  for (auto &el : j["frames"].items()) {
    frame.name = el.key();
    frame.duration = duration;
    frame.x = el.value()["frame"]["x"];
    frame.y = el.value()["frame"]["y"];
    frame.w = el.value()["frame"]["w"];
    frame.h = el.value()["frame"]["h"];
    frame.rotated = el.value()["rotated"];
    frame.trimmed = el.value()["trimmed"];
    frame.sourcePosX = el.value()["spriteSourceSize"]["x"];
    frame.sourcePosY = el.value()["spriteSourceSize"]["y"];
    frame.sourceW = el.value()["spriteSourceSize"]["w"];
    frame.sourceH = el.value()["spriteSourceSize"]["h"];
    frame.atlasFile = j["meta"]["image"];
    frame.atlasW = j["meta"]["size"]["w"];
    frame.atlasH = j["meta"]["size"]["h"];
    frameset.insert({frameId, frame});
    frameId += 1;
  }

  return frameset;
}

void rpg::AnimatedSprite::setOffsets() {
  auto width = animations_.at(current_anim_name_)->frameSet.at(currentFrame).w;
  auto height = animations_.at(current_anim_name_)->frameSet.at(currentFrame).h;

  offset_.x = -width * 0.5;
  offset_.y = -height * 0.5;
}
