#include "precompiled.h"
#include "NavSystem.h"
#include <Helpers.h>
#include "Globals.h"
#include "Engine.h"
#include <PlayerController.h>
#include <SelbaWard.hpp>
#include "Macroses.h"

rpg::NavSystem::NavSystem(std::string name, int id) : Object(name, id) {}

void rpg::NavSystem::draw(sf::RenderWindow *window) {

  if (!draw_debug_) {
    return;
  }
  std::string start = "#\n" + std::to_string((int) start_pos.x) + ":" + std::to_string((int) start_pos.y);
  //start = start.substr(0, start.find(".") + 3);
  text.setString(start);
  text.setPosition(start_pos.x, start_pos.y);
  window->draw(text);

  if (!navmesh_.empty()) {
    for (const auto &node : navmesh_) {

      if (node->step > 0) {
        auto heur = std::to_string(node->heuristics);
        auto heur_rounud = heur.substr(0, heur.find(".") + 3);
        std::string info = std::to_string(node->step) + "\n" + heur_rounud;
        text.setString(info.c_str());
        text.setPosition(node->position.x, node->position.y);
        window->draw(text);
      }
    }
  }

  if (!clean_path.empty()) {
    mathfu::Vector<float, 2> point_a, point_b;
    point_a = clean_path.front();
//  sw::Line line;

    for (int i = 1; i < clean_path.size(); ++i) {
      point_b = clean_path[i];
      sw::Line line({point_a.x, point_a.y}, {point_b.x, point_b.y}, 3, sf::Color::Blue);
      window_->draw(line);

      point_a = point_b;
    }
  }

  if (!optim_path.empty()) {
    mathfu::Vector<float, 2> point_a, point_b;
    point_a = optim_path.front();
//  sw::Line line;

    for (int i = 1; i < optim_path.size(); ++i) {
      point_b = optim_path[i];
      sw::Line line({point_a.x, point_a.y}, {point_b.x, point_b.y}, 2, sf::Color::Yellow);
      window_->draw(line);

      point_a = point_b;
    }
  }

}

void rpg::NavSystem::update(sf::Time dt) {

}

void rpg::NavSystem::handleEvent(const sf::Event &event) {

}

void rpg::NavSystem::onActivate() {

  window_ = RWIND;

  if (!font.loadFromFile("Assets/fonts/arial.ttf")) {
    // error...
  }

  text.setFont(font); // font is a sf::Font
  text.setCharacterSize(10); // in pixels, not points!
  text.setFillColor(sf::Color::White);
  text.setStyle(sf::Text::Regular);


//  for (const auto &node: navmesh_) {
//
//    std::string info =
//        std::to_string(node->tileId) + ", " +
//            node->tag + ", " +
//            std::to_string(node->navigable) + ", " +
//            std::to_string(node->id.first) + ", " +
//            std::to_string(node->id.second) + ", " +
//            std::to_string(node->position.x) + ", " +
//            std::to_string(node->position.y);

//    rpg::helper::printLog(info);


//  }
}

void rpg::NavSystem::removeNode(rpg::GridId id) {

}

std::vector<rpg::NavNode *> rpg::NavSystem::getNearest(float x, float y) {
  IPROF_FUNC;
  std::vector<mathfu::Vector<float, 2>> directions{
      {0.f, -1.f},  // North
      //{1.f, -1.f},  // North East
      {1.f, 0.f},   // East
      //{1.f, 1.f},   // South East
      {0.f, 1.f},   // South
      //{-1.f, 1.f},  // South West
      {-1.f, 0.f}  // West
      //{-1.f, -1.f}  // North West
  };

  std::vector<rpg::NavNode *> nodes{};

  for (int i = 0; i < directions.size(); ++i) {

    mathfu::Vector<float, 2> d(GRID, GRID);
    mathfu::Vector<float, 2> input(x, y);

    d *= directions[i];
    input += d;

    auto node = getNode(input.x, input.y);
    if (node != nullptr) {
      if (node->navigable && !node->visited) {
        nodes.push_back(node);
      }
    }
  }

  return nodes;
}

rpg::NavNode *rpg::NavSystem::getNode(float x, float y) {
  IPROF_FUNC;
//  int tileId = std::stoi(std::to_string((int) x / (int) GRID) + std::to_string((int) y / (int) GRID));
  int tileId = std::stoi(std::to_string(1000 + (int) x / (int) GRID) + std::to_string(1000 + (int) y / (int) GRID));

  auto result = std::find_if(navmesh_.begin(), navmesh_.end(),
                             [&](std::unique_ptr<NavNode> &i) { return i->tileId == tileId; });

//  printf("Tile ID %d\n", tileId);

  if (result == navmesh_.end()) {

    printf("Node not found %f, %f\n", x, y);
    return nullptr;
  }
  return result->get();
}

rpg::NavNode *rpg::NavSystem::getNode(int tileId) {
  IPROF_FUNC;
//  int tileId = std::stoi(std::to_string((int)x/(int)GRID) + std::to_string((int)y/(int)GRID));

  auto result = std::find_if(navmesh_.begin(), navmesh_.end(),
                             [&](std::unique_ptr<NavNode> &i) { return i->tileId == tileId; });

  if (result == navmesh_.end()) {

    printf("Node not found %d\n", tileId);
    return nullptr;
  }
  return result->get();
}

void rpg::NavSystem::addNode(std::unique_ptr<NavNode> node) {
  IPROF_FUNC;
  auto result = std::find_if(navmesh_.begin(), navmesh_.end(),
                             [&](std::unique_ptr<NavNode> &i) { return i->tileId == node->tileId; });
  if (result == navmesh_.end()) {
    navmesh_.push_back(std::move(node));
  }
//  else if(result != navmesh_.end() && result->tag != "Walls"){
//    result->tag = "Walls";
//  }
}

float rpg::NavSystem::calculateHeurisitc(int current_id, int start_id, int finish_id, int step) {
  IPROF_FUNC;
  float D = 1.0f;

  auto start = getNode(start_id);
  auto finish = getNode(finish_id);
  auto current = getNode(current_id);

  if (start == nullptr || finish == nullptr || current == nullptr) {
    return 0;
  }

  float h = D * (
      (std::abs((finish->position - current->position).Length())
          + std::abs((start->position - current->position).Length()) / (step + 1))
  );

  return h;
}

void rpg::NavSystem::getPath(int current_id, int start_id, int finish_id) {
  IPROF_FUNC;
  // node queue
  auto comparator = [this](NavNode *a, NavNode *b) { return ((a->heuristics > b->heuristics)); };
  std::priority_queue<NavNode *, std::vector<NavNode *>, decltype(comparator)> queue(comparator);
  std::priority_queue<NavNode *, std::vector<NavNode *>, decltype(comparator)> path(comparator);

  // initial values
  bool path_found = false;
  int step = 1;

  auto current = getNode(current_id);
  auto start = getNode(start_id);
  auto finish = getNode(finish_id);

  if (start == nullptr || finish == nullptr || current == nullptr) {
    return;
  }

  start->visited = true;
  start->heuristics = 999;


  // init Step 1 nodes
  auto nearNodes = getNearest(current->position.x, current->position.y);

  for (auto node: nearNodes) {
//    auto node = getNode(item);
    if (!node->visited) {
      node->visited = true;
      node->heuristics = calculateHeurisitc(node->tileId, start_id, finish_id, step);
      node->step = step;
      node->prevId = current->tileId;
      queue.push(node);
      path.push(node);
    }

  }

  // Step 2 nodes
  step = 2;

  while (!path_found && !queue.empty()) {

    current = queue.top();
    current->visited = true;
    queue.pop();
//    while(!queue.empty()){
//      queue.pop();
//    }

    auto nearNodes = getNearest(current->position.x + GRID * 0.5f, current->position.y + GRID * 0.5f);
    for (auto node : nearNodes) {

//      auto node = getNode(near_node);
      if (!node->visited) {
        node->heuristics = calculateHeurisitc(node->tileId, start_id, finish_id, step);
        node->visited = true;
        node->step = step;
        node->prevId = current->tileId;
        queue.push(node);
        path.push(node);
      }

      if (node->tileId == finish->tileId) {
        path_found = true;
        printf("Path found\n");
      }

    }

    step += 1;

  }

  move_path.push_front(finish->position);
  bool path_ready = false;
  auto n = finish->prevId;

  while (!path_ready) {

    auto p = getNode(n);

    if (p->tileId == start_id) {
      path_ready = true;
    } else {
      move_path.push_front(p->position + offset_);
      n = getNode(n)->prevId;
    }

  }

}

std::deque<mathfu::Vector<float, 2>> rpg::NavSystem::findPath(int actorId, float x, float y) {
  IPROF_FUNC;
//  helper::printLog("NAVIGATION ==========================");
//  for(auto node: navmesh_){
//
//    std::string info = std::to_string(node.tileId) + " " + node.tag;
//    helper::printLog(info);
//
//  }


  search_nodes.clear();
  std::deque<mathfu::Vector<float, 2>> empty;
  std::swap(move_path, empty);

  std::deque<mathfu::Vector<float, 2>> empty2;
  std::swap(clean_path, empty2);

  //printf("Searching %f, %f\n", x, y);
  for (auto &b : navmesh_) {
    b->visited = false;
    b->heuristics = 0;
    b->step = 0;
  }

//  path_found = false;
//  step = 0;
//  step += 1;
  auto actor = MGR->getEntity(actorId);
  auto start = actor->getPosition();
  start_pos = {start.x, start.y};

  if (getNode(start.x, start.y) == nullptr || getNode(x, y) == nullptr) {
    return clean_path;
  }

  auto nodeStart = getNode(start.x, start.y)->tileId;
  auto nodeFin = getNode(x, y)->tileId;

  if ((getNode(nodeFin)->tileId == getNode(nodeStart)->tileId) || !getNode(nodeFin)->navigable) {
    printf("No way\n");
  } else {
    getPath(nodeStart, nodeStart, nodeFin);
    //printf("Move path %zd\n", move_path.size());

    auto controller =
        dynamic_cast<rpg::PlayerController *>(MGR->getComponent(MGR->getEntity("Player")->id(), "Controller"));

//    if (move_path.size() >= 4) {
//      optimizePath(65);
//    }

    if (move_path.size() >= 1) {
//      cleanPath();
      move_path.push_back({x, y});
      optimizePath();
//      controller->setRoute(move_path);

//      controller->setRoute(clean_path);
    }

  }
  return clean_path;
}

void rpg::NavSystem::cleanPath() {
  IPROF_FUNC;
  for (int i = 1; i < move_path.size() - 1; ++i) {

    auto pointA = move_path[i - 1];
    auto pointB = move_path[i];
    auto pointC = move_path[i + 1];

    auto angle = "";

    auto ang = mathfu::Vector<float, 2>::Angle(pointB - pointA, pointC - pointA);

    if (ang != 0.f) {
      clean_path.push_back(pointB);
    }

    printf("Path: step %d: %f : %f, %f - %s\n", i, pointB.x, pointB.y, ang, angle);

  }
  clean_path.push_front(move_path.front());
  clean_path.push_back(move_path.back());

  printf("Length orig: %zd, optim: %zd\n", move_path.size(), clean_path.size());
}

void rpg::NavSystem::optimizePath() {
  IPROF_FUNC;
//  cleanPath();

  clean_path = move_path;

  if (clean_path.size() >= 3) {
    removeTurns();
  }

}

bool rpg::NavSystem::isVisible(mathfu::Vector<float, 2> a, mathfu::Vector<float, 2> b) {
  IPROF_FUNC;
  auto check_point = a;
  auto end_node = getNode(b.x, b.y);

  auto direct = (b - a).Normalized();
  auto step = direct * (GRID) * 0.25f;
  bool end = false;
  int steps = (b - a).Length() / step.Length();

  for (int i = 0; i < steps; ++i) {

    check_point += step;
    std::vector<NavNode *> node_points;
//    node_points.push_back(getNode(check_point.x, check_point.y));
//    node_points.push_back(getNode(check_point.x+32.f, check_point.y));
    node_points.push_back(getNode(check_point.x + 32.f, check_point.y + 32.f));
//    node_points.push_back(getNode(check_point.x, check_point.y+32.f));
    node_points.push_back(getNode(check_point.x - 32.f, check_point.y + 32.f));
//    node_points.push_back(getNode(check_point.x-32.f, check_point.y));
    node_points.push_back(getNode(check_point.x - 32.f, check_point.y - 32.f));
//    node_points.push_back(getNode(check_point.x, check_point.y-32.f));
    node_points.push_back(getNode(check_point.x + 32.f, check_point.y - 32.f));

    for (const auto &node : node_points) {
      if (node != nullptr && !node->navigable) {
//        printf("Not visible\n");
        return false;
      }
    }

//    if (node != nullptr && !node->navigable) {
//      printf("Not visible\n");
//      return false;
//    }
  }

  return true;
}

void rpg::NavSystem::removeTurns() {
  IPROF_FUNC;
  optim_path = clean_path;

  std::deque<mathfu::Vector<float, 2>> temp_path{};

  auto head = *clean_path.begin();
  temp_path.push_back(head);

  for (auto i = clean_path.begin() + 1; i != clean_path.end(); ++i) {
//    printf("Clear point %f, %f\n", i->x, i->y);
    if (isVisible(head, *i)) {
//      printf("Erase %f, %f\n", i->x, i->y);

    } else {
//      printf("Not visible reset Head\n");
      head = *i;
      temp_path.push_back(*std::prev(i, 1));
      temp_path.push_back(*i);
    }

//    printf("%f, %f\n", i->x, i->y);
  }

  temp_path.push_back(clean_path.back());
  temp_path.push_back(clean_path.back());

  std::swap(clean_path, temp_path);
}

mathfu::Vector<float, 2> rpg::NavSystem::getRandomNavigable(float x, float y, int radius) {
  IPROF_FUNC;
  auto rand_tile_x = helper::rand(-1 * radius, radius);
  auto rand_tile_y = helper::rand(-1 * radius, radius);

  auto pos_x = x + rand_tile_x;
  auto pos_y = y + rand_tile_y;

  auto start_point = mathfu::Vector<float, 2>(x, y);
  auto end_point = mathfu::Vector<float, 2>(pos_x, pos_y);

  auto node = getNode(pos_x, pos_y);

  if (node != nullptr && node->navigable) {
      return mathfu::Vector<float, 2>(pos_x, pos_y);
  }
  else{
    return mathfu::Vector<float, 2>(x, y);
  };

//  if (isVisible(start_point, end_point)) {
//    return mathfu::Vector<float, 2>(pos_x, pos_y);
//  } else {
//    printf("Try other point\n");
//    return mathfu::Vector<float, 2>(x, y);
//  }

}
