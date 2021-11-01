#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "bullets.hpp"
#include "alienship.hpp"
#include "playership.hpp"
#include "starlayers.hpp"


class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void initializeGL() override;
  void terminateGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void handleEvent(SDL_Event& event) override;

 private:
  int m_viewportHeight{};
  int m_viewportWidth{};

  GLuint m_starsProgram{};
  GLuint m_objectsProgram{};

  GameData m_gameData;
  Bullets m_bullets;
  PlayerShip m_player;
  ImFont* m_font{};
  abcg::ElapsedTimer m_restartWaitTimer;
  std::list<AlienShip> m_aliens;
  StarLayers m_starLayers;

  void restart();
  void update();
  void checkShot();
  void checkState(); 
};

#endif