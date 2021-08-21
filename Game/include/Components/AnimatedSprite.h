#pragma once
#include "precompiled.h"
#include "Component.h"
#include "ActorState.h"

namespace rpg {

FrameSet loadFrameSet(std::string path);


class AnimatedSprite : public rpg::Component, public sf::Transformable {
public:
  AnimatedSprite(int id, int owner, std::string name);
  ~AnimatedSprite() { onDestroy(); }

  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;

  // Event API
  void onActivate() override;
  void onDestroy() override;

  void setPath(std::string path);
  std::string getPath() { return path_; }
  void setAnimation(std::string name);
  void setAnimationByState(std::string state);
  std::string getCurrentAnim() { return current_anim_name_; }
  std::vector<std::string> getAnimations();

private:
//  void loadAnimation(std::string path);
  int loadAnimations(std::string path);
  void setFrame(int frameId, bool resetTime);
  void setOffsets();
  void switchAnimation();

private:
//  FrameSet frameSet;
//  sf::Texture texture;
//  sf::Sprite sprite;
//  sf::Vertex vertices[4];
//  bool isLooped{true};
//  bool isPaused{false};

  std::string path_;
  sf::Time frameTime;
  sf::Time currentTime;
  int currentFrame{0};
  rpg::Animation * currentAnim_;
  sf::Vector2f offset_;

  std::unordered_map<std::string, std::unique_ptr<rpg::Animation>> animations_;
  std::string current_anim_name_;

  friend class ActorState;
  std::string animState_;

};

}
