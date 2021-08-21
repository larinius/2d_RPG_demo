#include "precompiled.h"
#include "Tag.h"
#include "Helpers.h"
#include "Globals.h"
#include "Macroses.h"

rpg::TagFactory::TagFactory() = default;

std::map<int, rpg::Tag> rpg::TagFactory::tags_;

int rpg::TagFactory::registerTag(std::string alias) {
  Tag t;
  int id = ID;
  t.id = id;
  t.alias = alias;
  t.isActive = true;
  rpg::TagFactory::tags_.insert({id, t});
  return id;
}

rpg::Tag rpg::TagFactory::getTag(int id) {
  return tags_.at(id);
}

rpg::Tag rpg::TagFactory::getTag(std::string alias) {
  auto result = std::find_if(tags_.begin(), tags_.end(),
                             [alias](const std::pair<int, Tag> &i) { return i.second.alias == alias; });
  return result->second;
}

std::vector<rpg::Tag> rpg::TagFactory::getTags() {
  std::vector<Tag> t;
  t.reserve(tags_.size());
  for (const auto &s : tags_)
    t.push_back(s.second);

  return t;
}

void rpg::TagFactory::clearTags() {
  tags_.clear();
}
