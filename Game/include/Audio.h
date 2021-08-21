#pragma once
#include "precompiled.h"
#include "Object.h"

namespace rpg {

class Audio : public Object {
public:
  Audio(std::string name, int id);
  ~Audio() {onDestroy();}


  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt)  override;
  void handleEvent(const sf::Event &event)  override;

  int addAudio(std::string path, std::string name);
  void playAudio(int id, bool loop, float volume);
  void playAudio(std::string name, bool loop, float volume);
  void stopAudio();
  void stopAudio(int id);
  void stopAudio(std::string name);

  MusicObject *getAudio(int id);
  MusicObject *getAudio(std::string name);

private:
  void onActivate() override;
  void onDestroy() override;

private:
  std::vector<std::unique_ptr<rpg::MusicObject>> tracks_;
};

}
