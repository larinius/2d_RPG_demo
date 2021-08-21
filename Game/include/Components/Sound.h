#pragma once
#include "Component.h"

namespace rpg {

class Sound : public Component {

public:
  Sound(int id, int owner, std::string name);
  ~Sound() { onDestroy(); }

  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;

private:
  void onActivate() override;
  void onDestroy() override;

public:
  int addSound(std::string name, std::string path, bool loop=false, float volume=100.f);
  void playSound(int id);
  void playSound(std::string name);
  void stopSound(int id);
  void stopSound(std::string name);
  void stopAll();

private:
  SoundObject* getSound(int id);
  SoundObject* getSound(std::string name);

private:
  std::deque<std::unique_ptr<rpg::SoundObject>> sounds_;
};

}
