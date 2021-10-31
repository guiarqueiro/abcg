#ifndef ALIEN_HPP_
#define ALIEN_HPP_

#include <random>

#include "abcg.hpp"
#include "defaultvalues.hpp"
#include "base_ship.hpp"
#include "gamedata.hpp"
#include "randomizer.hpp"

class AlienShip : public BaseShip {
  const Type objType = Type::Alien;

 public:
  void initializeGL(GLuint program, float verticalPosition);
  void paintGL(const GameData &gameData) override;
  void terminateGL() override;
  void takeDamage() override;
  void update(float deltaTime);

 private:
  abcg::ElapsedTimer m_bulletCoolDownTimer;
  bool m_inInvertRegionFlag{false};

  void updatePosition(float deltaTime);
  void updateShooting();
  float getRandomNumber(float start, float end, bool isNegativeAcceptable);
};

#endif