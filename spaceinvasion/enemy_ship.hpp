#ifndef ENEMY_HPP_
#define ENEMY_HPP_

#include <random>

#include "abcg.hpp"
#include "base_constants.hpp"
#include "base_ship.hpp"
#include "gamedata.hpp"
#include "util.hpp"

class EnemyShip : public BaseShip {
  const int LIFE_POINTS = 3;
  const float MIN_VELOCITY = 0.5f;
  const float MAX_VELOCITY = 1.0f;
  const float NORMALIZE_FACTOR = 10.0f;
  const Type SHIP_TYPE = Type::Enemy;
  const std::array<glm::vec2, 8> BODY_POINTS{
      glm::vec2{-10.0f, +03.0f} /*0*/, glm::vec2{-10.0f, -03.0f} /*1*/,
      glm::vec2{+0.0f, -03.0f} /*2*/,  glm::vec2{+10.0f, -03.0f} /*3*/,
      glm::vec2{+10.0f, +03.0f} /*4*/, glm::vec2{+05.0f, +00.0f} /*5*/,
      glm::vec2{+00.0f, +03.0f} /*6*/, glm::vec2{-05.0f, +00.0f} /*7*/};
  const std::array<int, 18> BODY_POINTS_INDICES{0, 1, 7, 1, 7, 2, 7, 6, 2,
                                                2, 6, 5, 2, 5, 3, 3, 5, 4};

 public:
  void paintGL(const GameData &gameData) override;
  void terminateGL() override;
  void receiveDamage() override;

  void initializeGL(GLuint program, float verticalPosition);
  void update(float deltaTime);

 private:
  abcg::ElapsedTimer m_bulletCoolDownTimer;
  std::array<glm::vec2, 7> getBodyPositions();
  std::array<int, 18> getBodyIndices();
  float m_invertDelta{0.10f};
  bool m_inInvertRegionFlag{false};

  void updatePosition(float deltaTime);
  void updateShooting();
  float getRandomNumber(float start, float end, bool isNegativeAcceptable);
};

#endif