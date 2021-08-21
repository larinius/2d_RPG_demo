#include "Container.h"

namespace rpg {

template<typename T>
Container::Container() {
}

template<typename T>
Container::~Container() {
}

template<typename T>
void Container::insert(Item &i) {
  box.push_back(i);
}

template<typename T>
Item<T> Container::get(const char *id) {

  for (int i = 0; i < box.size(); ++i) {
    auto _c = box[i].key;
    if (strcmp(_c, id) == 0)
      return box[i];
  }

  return box.back();
}

//template<typename T>
//const Item &Container<Level>::get(const char *id) const {
////  return <#initializer#>;
//  return NULL;
//}


template<typename T>
int Container::count() {
  int c;
  c = box.size();
  return c;
}

}