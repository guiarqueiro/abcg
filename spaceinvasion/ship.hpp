#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "abcg.hpp"
#include "base_constants.hpp"
#include "base_ship.hpp"
#include "gamedata.hpp"

class Ship : public BaseShip {
  const int LIFE_POINTS = 5;
  const float NORMALIZE_FACTOR = 15.5f;
  const float VERTICAL_POSITION = -0.82f;
  const std::array<glm::vec2, 10> BODY_POINTS{
      glm::vec2{-02.5f, +02.5f} /*0*/, glm::vec2{-15.5f, -02.5f} /*1*/,
      glm::vec2{-15.5f, -12.5f} /*2*/, glm::vec2{-09.5f, -11.0f} /*3*/,
      glm::vec2{-03.5f, -12.5f} /*4*/, glm::vec2{+03.5f, -12.5f} /*5*/,
      glm::vec2{+09.5f, -11.5f} /*6*/, glm::vec2{+15.5f, -12.5f} /*7*/,
      glm::vec2{+15.5f, -02.5f} /*8*/, glm::vec2{+02.5f, +02.5f}} /*9*/;

  const std::array<int, 24> BODY_POINTS_INDICES{
      0, 1, 3, 1, 2, 3, 0, 3, 4, 0, 4, 5, 9, 0, 5, 9, 5, 6, 9, 6, 8, 8, 6, 7};

 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData) override;
  void terminateGL() override;
  void update(float deltaTime);
  void receiveDamage() override;

  float m_lastMousePosition{0};
  float m_mouseMovementDelta{0.01f};
  abcg::ElapsedTimer m_trailBlinkTimer;

 private:
  bool isWithinMouseInterval();
};

#endif