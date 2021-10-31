#ifndef HPBAR_HPP_
#define HPBAR_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"
#include "playership.hpp"
#include "base_ship.hpp"

class HpBar {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL() ;
  void update(float deltaTime);
  void receiveDamage();

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};


  PlayerShip m_playership;
  float m_scale{1.0f};
};

#endif