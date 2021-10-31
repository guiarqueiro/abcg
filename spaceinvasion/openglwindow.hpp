#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "bullets.hpp"
#include "alienship.hpp"
#include "playership.hpp"
//#include "hpbar.hpp"

const int AlienQtt = 5;
const float AlienDistance = 0.20f;


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
  int m_lastmousepos{0};
  int m_viewportHeight{};
  int m_viewportWidth{};

  abcg::ElapsedTimer m_restartWaitTimer;
  std::list<AlienShip> m_aliens;
  GameData m_gameData;
  Bullets m_bullets;
  ImFont* m_font{};
  PlayerShip m_player;
  //HpBar m_hpbar;

  void verifyShot();
  void verifyState();

  void restart();
  void update();
};

#endif