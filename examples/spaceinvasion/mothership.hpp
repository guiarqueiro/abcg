#ifndef MOTHERSHIP_HPP_
#define MOTHERSHIP_HPP_

#include "abcg.hpp"
#include "actiondata.hpp"
#include "gamedata.hpp"
#include "typedata.hpp"

class Bullets;
class OpenGLWindow;

class MotherShip {
 public:
  virtual ~MotherShip() = default;
  virtual void paintGL(const GameData &gameData) = 0;
  virtual void terminateGL() = 0;
  virtual void takeDamage() = 0;
  void lifeStatus();

 protected:
  friend Bullets;
  friend OpenGLWindow;


  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  ActionData m_actionData;
  TypeData m_typeData;
  abcg::ElapsedTimer m_bulletCoolDownTimer;
  glm::vec4 m_color{1};
  float m_scale{0.150f};
  int m_hpBase{};
  int m_currentLifePoints{};
  glm::vec2 m_direction{0.0f, 1.0f};
  glm::vec2 m_translation{glm::vec2(0)};
  glm::vec2 m_velocity{glm::vec2(0)};

};

#endif