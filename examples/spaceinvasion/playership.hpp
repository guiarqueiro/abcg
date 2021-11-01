#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "mothership.hpp"
#include "gamedata.hpp"

class PlayerShip : public MotherShip {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData) override;
  void terminateGL() override;
  void update(float deltaTime);
  void takeDamage() override;

  float m_lastMousePosition{0};

 private:
  bool reachMouse();
};

#endif