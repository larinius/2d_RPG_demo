#include "Audio.h"
#include "Globals.h"
#include "Macroses.h"

rpg::Audio::Audio(std::string name, int id) : Object(name, id) {

}

void rpg::Audio::draw(sf::RenderWindow *window) {

}

void rpg::Audio::update(sf::Time dt) {

}

void rpg::Audio::handleEvent(const sf::Event &event) {

}

void rpg::Audio::onActivate() {

}

int rpg::Audio::addAudio(std::string path, std::string name = "") {

  auto sound = std::make_unique<MusicObject>();

  int id = ID;
  sound->id = id;
  sound->path = path;
  sound->name = !name.empty() ? name : "Sound_" + std::to_string(id);

  tracks_.push_back(std::move(sound));

  return id;
}

void rpg::Audio::playAudio(int id, bool loop=false, float volume = 100.f) {

  auto m = getAudio(id);
  m->music.openFromFile(m->path);
  m->music.setVolume(volume);
  m->music.setLoop(loop);
  m->music.play();
}

void rpg::Audio::playAudio(std::string name, bool loop=false, float volume = 100.f) {

  auto m = getAudio(name);
  m->music.openFromFile(m->path);
  m->music.setVolume(volume);
  m->music.setLoop(loop);
  m->music.play();
}

rpg::MusicObject *rpg::Audio::getAudio(int id) {
  auto result = std::find_if(tracks_.begin(), tracks_.end(),
                             [&](std::unique_ptr<MusicObject> &i) { return i->id == id; });

  return result != tracks_.end() ? result->get() : nullptr;
}

rpg::MusicObject *rpg::Audio::getAudio(std::string name) {
  auto result = std::find_if(tracks_.begin(), tracks_.end(),
                             [&](std::unique_ptr<MusicObject> &i) { return i->name == name; });

  return result != tracks_.end() ? result->get() : nullptr;
}

void rpg::Audio::onDestroy() {
  stopAudio();
}

void rpg::Audio::stopAudio() {
  for(const auto & m : tracks_){
    if(m->music.getStatus() == sf::SoundSource::Status::Playing){
      m->music.stop();
    }
  }
}

void rpg::Audio::stopAudio(int id) {
  getAudio(id)->music.stop();
}

void rpg::Audio::stopAudio(std::string name) {
  getAudio(name)->music.stop();
}
