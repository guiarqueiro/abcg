#include "playership.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void PlayerShip::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_hpBase = 5;
  m_currentLifePoints = m_hpBase;
  m_velocity = glm::vec2(0);
  m_translation = glm::vec2(0, -0.70f);
  

  std::array<glm::vec2, 10> positions{
      glm::vec2{-02.5f, +02.5f} /*0*/, glm::vec2{-15.5f, -02.5f} /*1*/,
      glm::vec2{-15.5f, -12.5f} /*2*/, glm::vec2{-09.5f, -11.0f} /*3*/,
      glm::vec2{-03.5f, -12.5f} /*4*/, glm::vec2{+03.5f, -12.5f} /*5*/,
      glm::vec2{+09.5f, -11.5f} /*6*/, glm::vec2{+15.5f, -12.5f} /*7*/,
      glm::vec2{+15.5f, -02.5f} /*8*/, glm::vec2{+02.5f, +02.5f}} /*9*/;

  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f}; 
  }

  std::array<int, 24> indices{
      0, 1, 3, 1, 2, 3, 0, 3, 4, 0, 4, 5, 9, 0, 5, 9, 5, 6, 9, 6, 8, 8, 6, 7};

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

  glGenVertexArrays(1, &m_vao);

  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  glBindVertexArray(0);
}

void PlayerShip::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);
  glBindVertexArray(m_vao);
  glUniform1f(m_scaleLoc, m_scale);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);
  
  m_color = {0.153f, 0.723f, 0.102f, 1.0f};

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 10 * 3, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  glUseProgram(0);
}

void PlayerShip::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void PlayerShip::update(float deltaTime) {
  if (m_actionData.m_input[static_cast<size_t>(Action::MouseLeft)] &&
      m_translation.x > -1 + m_scale && !reachMouse()) {
    m_translation -= glm::vec2(1, 0) * deltaTime;
  }
  if (m_actionData.m_input[static_cast<size_t>(Action::MouseRight)] &&
      m_translation.x < 1 - m_scale && !reachMouse()) {
    m_translation += glm::vec2(1, 0) * deltaTime;
  }
}

void PlayerShip::takeDamage() { m_currentLifePoints--; }

bool PlayerShip::reachMouse() {
  float space = glm::abs(m_lastMousePosition - m_translation.x);
  return space < 0.01f;
}
