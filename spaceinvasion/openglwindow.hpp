#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "bullets.hpp"
#include "enemy_ship.hpp"
#include "ship.hpp"

const float ENEMY_SHIP_START_POSITION = 0.90f;  // alien_startPos
const float ENEMY_SHIP_VERTICAL_DELTA = 0.20f;  // alien_defaultDistance
const int ENEMY_SHIP_QUANTITY = 5;              // alien_qtt

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void resizeGL(int width, int height) override;
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void terminateGL() override;
  void paintGL() override;
  void paintUI() override;

 private:
  GLuint m_objectsProgram{};
  int m_lastmousepos{300};
  int m_viewportHeight{};
  int m_viewportWidth{};

  abcg::ElapsedTimer m_restartWaitTimer;
  std::list<EnemyShip> m_enemies;  // <Alien>
  GameData m_gameData;
  Bullets m_bullets;
  ImFont* m_font{};
  Ship m_ship;

  void verifyEnemyLife();
  void verifyGameOver();
  void verifyShot();
  void verifyWin();
  void restart();
  void update();
};

#endif