#pragma once
#include "precompiled.h"
#include "Object.h"

namespace rpg {

class NavSystem : public Object {
public:
  NavSystem(std::string name, int id);
  NavSystem(const NavSystem &) = delete;
  NavSystem &operator=(const NavSystem &) = delete;
  ~NavSystem(){onDestroy();}
  // Common API
  void draw(sf::RenderWindow *window) override;
  void update(sf::Time dt) override;
  void handleEvent(const sf::Event &event) override;
  void onActivate() override;
  void onDestroy() override {}

  // Objects API
  void addNode(std::unique_ptr<NavNode> node);
  void removeNode(GridId id);

  std::vector<rpg::NavNode *> getNearest(float x, float y);
  rpg::NavNode *getNode(float x, float y);
  rpg::NavNode *getNode(int tileId);

  void getPath(int current, int start, int finish);
  float calculateHeurisitc(int current, int start, int finish, int step);
  void optimizePath();
  void cleanPath();
  void removeTurns();

  bool isVisible(mathfu::Vector<float, 2> a, mathfu::Vector<float, 2> b);

  std::deque<mathfu::Vector<float, 2>> findPath(int actorId, float x, float y);
  mathfu::Vector<float, 2> getRandomNavigable(float x, float y, int radius);

  mathfu::Vector<float, 2> start_pos;

private:
  std::vector<std::unique_ptr<NavNode>> navmesh_{};
  std::vector<NavNode *> search_nodes{};

  std::deque<mathfu::Vector<float, 2>> move_path{};
  std::deque<mathfu::Vector<float, 2>> optim_path{};
  std::deque<mathfu::Vector<float, 2>> clean_path{};

  mathfu::Vector<float, 2> offset_{GRID*0.5, GRID*0.5};

  sf::RenderWindow *window_;
  sf::Font font;
  sf::Text text;
  bool draw_debug_{false};


};

}
