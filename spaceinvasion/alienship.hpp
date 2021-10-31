#ifndef ALIEN_HPP_
#define ALIEN_HPP_

#include <random>

#include "abcg.hpp"
#include "defaultvalues.hpp"
#include "base_ship.hpp"
#include "gamedata.hpp"
#include "randomizer.hpp"

class AlienShip : public BaseShip {
  const Type SHIP_TYPE = Type::Alien;

 public:
  void initializeGL(GLuint program, float verticalPosition);
  void paintGL(const GameData &gameData) override;
  void terminateGL() override;
  void receiveDamage() override;
  void update(float deltaTime);

 private:
  abcg::ElapsedTimer m_bulletCoolDownTimer;
  std::array<glm::vec2, 7> getBodyPositions();
  std::array<int, 18> getBodyIndices();
  bool m_inInvertRegionFlag{false};

  void updatePosition(float deltaTime);
  void updateShooting();
  float getRandomNumber(float start, float end, bool isNegativeAcceptable);
};

#endif