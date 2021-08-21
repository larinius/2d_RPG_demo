#include "precompiled.h"
#include "Components/Sound.h"
#include "Helpers.h"
#include "Macroses.h"

rpg::Sound::Sound(int id, int owner, std::string name) : Component(id, owner, name) {

}

///////////////////////////////////////////////
// Override methods
///////////////////////////////////////////////

void rpg::Sound::draw(sf::RenderWindow *window) {

}

void rpg::Sound::update(sf::Time dt) {

}

void rpg::Sound::handleEvent(const sf::Event &event) {

}

void rpg::Sound::onActivate() {

}

void rpg::Sound::onDestroy() {

  while(!sounds_.empty()){
    sounds_.front().release();
    sounds_.pop_front();
  }
}
///////////////////////////////////////////////
// Class methods
///////////////////////////////////////////////

rpg::SoundObject *rpg::Sound::getSound(int id) {
  auto result = std::find_if(sounds_.begin(), sounds_.end(),
                             [&](std::unique_ptr<SoundObject> &i) { return i->id == id; });

  return result != sounds_.end() ? result->get() : nullptr;
}

rpg::SoundObject *rpg::Sound::getSound(std::string name) {
  auto result = std::find_if(sounds_.begin(), sounds_.end(),
                             [&](std::unique_ptr<SoundObject> &i) { return i->name == name; });

  return result != sounds_.end() ? result->get() : nullptr;
}

int rpg::Sound::addSound(std::string name, std::string path, bool loop, float volume) {

  auto so = std::make_unique<rpg::SoundObject>();
  int id = ID;
  so->id = id;
  so->name = name;
  so->path = path;

  if (!so->buffer.loadFromFile(so->path)) {
//    printf("Sound Load Error: %s\n", path.c_str());
    return 0;
  }

  so->sound.setBuffer(so->buffer);
  so->sound.setLoop(loop);
  so->sound.setVolume(volume);

  sounds_.push_front(std::move(so));

  return id;
}

void rpg::Sound::playSound(int id) {
  getSound(id)->sound.play();
}

void rpg::Sound::playSound(std::string name) {
  getSound(name)->sound.play();
}

void rpg::Sound::stopSound(int id) {
  getSound(id)->sound.stop();
}

void rpg::Sound::stopSound(std::string name) {
  getSound(name)->sound.stop();
}

void rpg::Sound::stopAll() {
  for (const auto &s :sounds_) {
    s->sound.stop();
  }
}
