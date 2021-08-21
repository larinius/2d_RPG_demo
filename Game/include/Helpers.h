#pragma once
#include "precompiled.h"

#define START_ID 10000

namespace rpg::helper {

class Id {
public:
  Id();

  static int getId();
  static bool is_valid(int id);
  static void reset();

private:
  static std::vector<int> ids_;
};

/**
 * Generates string filled with random decimal numbers.
 * @param length
 * @return
 */
std::string randomNumString(int length);

/**
 * Loads data from json file
 * @param path - Path to json file
 * @param key - Key from json to read (example: "asset")
 * @return data string from key specified key
 */
std::string loadJson(std::string const &path, std::string const &key);

/**
 * Prints string "msg" into "log.txt"
 * @param msg
 */
void printLog(const std::string msg);

std::string str2key(std::string str);

/**
 * Removes specific int item from vector, returns new vector
 * @param vec
 * @param item
 * @return new modified vector<int>
 */
std::vector<int> vec_remove(std::vector<int> vec, int item);

bool isPathValid(std::string path);

int rand(int min, int max);

}
