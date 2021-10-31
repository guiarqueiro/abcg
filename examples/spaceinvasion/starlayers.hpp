#ifndef STARLAYERS_HPP_
#define STARLAYERS_HPP_

#include <array>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"

class OpenGLWindow;

class StarLayers {
 public:
  void initializeGL(GLuint program, int quantity);
  void paintGL();
  void terminateGL();
  void update(float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_pointSizeLoc{};
  

  struct StarLayer {
    GLuint m_vao{};
    GLuint m_vbo{};

    float m_pointSize{};
    int m_quantity{};
    glm::vec2 m_translation{glm::vec2(0)};
  };

  std::array<StarLayer, 3> m_starLayers;

  std::default_random_engine m_randomEngine;
};

#endif