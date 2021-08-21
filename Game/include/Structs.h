#pragma once
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
//#include "Entity.h"

namespace rpg {

struct Behaviors {
  std::string name;
  int timeout;
  bool repeat;
  int radius;
  float priority;
};

using GridId = std::pair<int, int>;

struct NavNode {
  std::string tag = "";
  GridId id;
  int tileId = 0;
  int prevId = 0;
  mathfu::Vector<float, 2> position{0.f, 0.f};
  mathfu::Vector<float, 2> size{0.f, 0.f};
  float cost = 0;
  bool navigable = false;
  float heuristics = 0.f;
  bool visited = false;
  int step = 0;
  bool turn_point = false;
};

struct PathNode {
  NavNode node;
  NavNode next;
};

struct MusicObject {
  sf::Music music;
  int id = 0;
  std::string name = "";
  std::string path = "";
  bool play = true;
  bool loop = false;
};

class Entity;
struct Actor {
  int npcId = 0;
  rpg::Entity *entity = nullptr;
  std::string npcName = "";
  int timout = 0;
  Behavior behavior = Behavior::idle;
};

struct AnimationFrame {
  std::string atlasFile;
  std::string name;
  int duration;
  int x;
  int y;
  int w;
  int h;
  bool rotated;
  bool trimmed;
  int sourcePosX;
  int sourcePosY;
  int sourceW;
  int sourceH;
  int atlasW;
  int atlasH;
};

struct Command {
  int id;                       //Unique command id for internal use
  int listener_id;              //Who executes this command
  int target_id;                //Target of the command (Entity)
  Action action;                //Action to execute (Enum class)
  mathfu::Vector<float, 2> pos; //Used if command needs coordiantes like "move to"
  int timout;                   //Timer for commnd life, if 0, command removed
  bool repeat;                  //Repeat command when finished
  Behavior behavior;            //Behavior type enum
  std::string parameter;        //Optional string parameter
};

using FrameSet = std::map<int, AnimationFrame>;

struct Animation {
  int id;
  std::string name;
  std::string path;
  std::string state;
  bool isLooped = true;
  bool isPaused = false;
  FrameSet frameSet;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Vertex vertices[4];
};

struct SoundObject {
  int id;
  std::string name;
  std::string path{""};
  sf::SoundBuffer buffer;
  sf::Sound sound;
};

struct GuiSoundObject {
  std::string name;
  std::string path{""};
  sf::SoundBuffer buffer;
  sf::Sound sound;
};

struct Body {
  int id;
  std::string name;
  bool is_active;
  b2BodyDef bodyDef;
  b2Body *body;
//  std::unique_ptr<b2Body> body;
  b2PolygonShape boxShape;
  b2CircleShape circleShape;
};

struct Tag {
  int id;
  std::string alias;
  bool isActive = false;
};

}
