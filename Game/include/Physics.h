#pragma once
#include "precompiled.h"
#include <Box2D/Box2D.h>
#include "SFMLDebugDraw.h"
#include "Object.h"

namespace rpg {

class QueryCallback : public b2QueryCallback {
public:
  QueryCallback(const b2Vec2 &point) {
    m_point = point;
    m_fixture = NULL;
  }

  bool ReportFixture(b2Fixture *fixture) {
    b2Body *body = fixture->GetBody();
    if (body->GetType() == b2_dynamicBody) {
      bool inside = fixture->TestPoint(m_point);
      if (inside) {
        m_fixture = fixture;

        // We are done, terminate the query.
        return false;
      }
    }

    // Continue the query.
    return true;
  }

  b2Vec2 m_point;
  b2Fixture *m_fixture;
};


class Physics : public Object {
public:
  Physics(std::string name, int id);
  ~Physics() { onDestroy(); }

  void update(sf::Time dt) override;
  void draw(sf::RenderWindow *window) override;
  void handleEvent(const sf::Event &event) override;

  /**
   * Create collision box for characters and containers and other movable objects.
   */
  rpg::Body *createAABB(std::string name = "AABB", float posX = 0.f, float posY = 0.f, float size = GRID);

  /**
   * Create collision box for walls, floors and other static objects.
   */
  rpg::Body *createStatic(std::string name = "Wall",
                          float posX = 0.f,
                          float posY = 0.f,
                          float sizeX = GRID,
                          float sizeY = GRID);

  void clear();
private:
  void onActivate() override;
  void onDestroy() override;

private:
  b2Vec2 gravity_{0.0f, 0.0f};
//  b2World world_{gravity_};
  std::unique_ptr<b2World> world_;

  float timeStep{1.0f / 60.0f};
  int32 velocityIterations_{6};
  int32 positionIterations_{2};

  std::vector<std::shared_ptr<rpg::Body>> objects_;

  bool drawDebug_{false};
  sf::RenderWindow *window_;
  std::unique_ptr<SFMLDebugDraw> debugDraw_;
};

}
