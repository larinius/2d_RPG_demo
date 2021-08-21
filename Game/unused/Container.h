#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iterator>
#include "Helpers.h"

namespace rpg {

template<typename T>
struct Item{
  Item(T& obj, std::string key, bool isActive):obj(obj), key(key), isActive(isActive){}

  std::string key;
  bool isActive = false;
  T obj;
};


template<typename T>
class Container {

public:

  bool insert(Item<T> & obj) {

//    Item<T> itm;
//    itm.key(key);
//    itm.obj(obj);
    box.push_back(obj);

    return true;
  }

  Item<T> &get(const std::string key) {

    auto result = std::find_if(box.begin(), box.end(),
                               [&](Item<T> const &i) { return i.key == key; });
    return *result;
  }

  int count() {
    int c;
    c = box.size();
    return c;
  }

private:
  std::vector<Item<T>> box;
};

}

