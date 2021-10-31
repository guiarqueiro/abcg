#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "defaultvalues.hpp"
#include "base_ship.hpp"
#include "gamedata.hpp"

class PlayerShip : public BaseShip {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData) override;
  void terminateGL() override;
  void update(float deltaTime);
  void receiveDamage() override;

  float m_lastMousePosition{0};

 private:
  bool reachMouse();
};

#endif