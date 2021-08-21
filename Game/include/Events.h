#pragma once
#include "precompiled.h"
#include "eventpp/eventqueue.h"
#include "eventpp/utilities/orderedqueuelist.h"

namespace rpg {

enum EventsTypes {
  guiButtonPress = 100,

};


using EQueue = eventpp::EventQueue<int, void(std::string, const bool)>;

class Events {
public:
  void Event();
  void Update();

private:
  EQueue queue;

private:
  void AddListener(EventsTypes e, const std::string listener);

  void tutorial();

};

}
