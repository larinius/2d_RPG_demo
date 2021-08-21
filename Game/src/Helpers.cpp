#include "precompiled.h"
#include "Helpers.h"
#include "Globals.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/locid.h>
//#include <reckless/severity_log.hpp>
//#include <reckless/file_writer.hpp>
#include <nlohmann/json.hpp>

std::string rpg::helper::randomNumString(int length) {
  typedef std::mt19937::result_type PRNG;
  using namespace std::chrono;
  std::random_device rd;
  PRNG seed = rd() ^ ((PRNG) duration_cast<seconds>(system_clock::now().time_since_epoch()).count() +
      (PRNG) duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count());

  std::mt19937 gen(seed);
  std::uniform_int_distribution<unsigned> distrib(1, 6);

  std::string out;
  for (unsigned long j = 0; j < length; ++j)
    out += std::to_string(distrib(gen));

  return out;
}

std::string rpg::helper::loadJson(std::string const &path, std::string const &key) {

  FILE *fp;
  fopen_s(&fp, path.c_str(), "rb"); // non-Windows use "r"

  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

  rapidjson::Document d;
  d.ParseStream(is);

  auto data = d[key.c_str()].GetString();

  printf("Json = %s\n", data);

  fclose(fp);

  return data;
}

void rpg::helper::printLog(std::string msg) {

//  using log_t = reckless::severity_log<
//      reckless::indent<4>,       // 4 spaces of indent
//      ' ',                       // Field separator
//      reckless::severity_field,  // Show severity marker (D/I/W/E) first
//      reckless::timestamp_field  // Then timestamp field
//  >;
//
//  reckless::file_writer writer("log.txt");
//  log_t g_log(&writer);
//
//  g_log.info("Info : %s", msg);

}

std::string rpg::helper::str2key(std::string str) {

  icu::UnicodeString s(str.c_str(), "UTF-8");
  icu::UnicodeString fill("_", "UTF-8");
  std::string s2; // Output string

  s.trim();
  s.toLower();

  s.findAndReplace(" ", fill);

  s.append(fill);
  s.append(rpg::helper::randomNumString(4).c_str());

  s.toUTF8String(s2);

  return s2;
}

std::vector<int> rpg::helper::vec_remove(std::vector<int> vec, int item) {
  vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const int &x) {
    return x == item;
  }), vec.end());

  return vec;
}

std::vector<int> rpg::helper::Id::ids_;

struct compare {
  int id;
  compare(int const &i) : id(i) {}
  bool operator()(int const &i) {
    return (i == id);
  }
};

int rpg::helper::Id::getId() {

  if (ids_.empty()) {
    int new_id = START_ID;
    ids_.push_back(new_id);
    return new_id;
  } else {
    int new_id = ids_.back();
    ++new_id;
    ids_.push_back(new_id);
    return new_id;
  }

  return START_ID;
}

bool rpg::helper::Id::is_valid(const int id) {

  if (std::find_if(Id::ids_.begin(), Id::ids_.end(), compare(id)) != Id::ids_.end()) {
    return true;
  } else {
    return false;
  }

}

rpg::helper::Id::Id() {
  ids_.push_back(START_ID);
}

void rpg::helper::Id::reset() {
  ids_.clear();
}

bool rpg::helper::isPathValid(std::string path) {

  if (path.empty()) {
    printf("Path is empty!\n");
    return false;
  }

  const std::filesystem::path p = path;
  bool valid = std::filesystem::exists(p);
  if(!valid){
    printf("Path not valid!: %s\n", path.c_str());
    return false;
  }

  return true;
}

int rpg::helper::rand(int min, int max){
  typedef std::mt19937::result_type PRNG;
  using namespace std::chrono;
  std::random_device rd;
  PRNG seed = rd() ^ ((PRNG) duration_cast<seconds>(system_clock::now().time_since_epoch()).count() +
      (PRNG) duration_cast<microseconds>(high_resolution_clock::now().time_since_epoch()).count());

  std::mt19937 gen(seed);
  std::uniform_int_distribution<int> distrib(min, max);

  return distrib(gen);
}
