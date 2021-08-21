#pragma once
#include "precompiled.h"

namespace rpg {

class TagFactory {
public:
  TagFactory();
  TagFactory(const TagFactory &) = delete;
  TagFactory& operator=(const TagFactory &) = delete;
  ~TagFactory() = default;

  static int registerTag(std::string alias);
  static Tag getTag(int id);
  static Tag getTag(std::string alias);
  static std::vector<rpg::Tag> getTags();
  void clearTags();

private:
  static std::map<int, rpg::Tag> tags_;
};

}
