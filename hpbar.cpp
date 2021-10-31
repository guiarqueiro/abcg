#include "hpbar.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void HpBar::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_playership.m_baseLifePoints = 5;
  m_playership.m_currentLifePoints = m_playership.m_baseLifePoints;
  m_playership.m_translation = glm::vec2(0, -0.86f);
  m_playership.m_velocity = glm::vec2(0);

  std::array<glm::vec2, 4> positions{
      glm::vec2{-300.0f, +02.5f} /*0*/, glm::vec2{-300.0f, -02.5f} /*1*/,
      glm::vec2{+300.0f, -02.5f} /*2*/, glm::vec2{+300.0f, +02.5f} /*3*/;

  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  std::array<int, 6> indices{0, 1, 2, 2, 3, 0};

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

void HpBar::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);
  glBindVertexArray(m_vao);
  glUniform1f(m_scaleLoc, m_scale);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);

  float lifePercentage = 1.0f * m_playership.m_currentLifePoints / 5;

  if (lifePercentage > 0.75f) {
    m_color = BaseConstants::GREEN_COLOR;
  } else if (lifePercentage > 0.35f) {
    m_color = BaseConstants::YELLOW_COLOR;
  } else {
    m_color = BaseConstants::RED_COLOR;
  }

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 10 * 3, GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
  glUseProgram(0);
}

void HpBar::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void HpBar::update(float deltaTime) {
  float lifePercentage = 1.0f * m_playership.m_currentLifePoints / 5;
  if (lifePercentage > 0.75f){
      abcg::glUniform1f(m_scaleLoc, m_scale);
  }
}

