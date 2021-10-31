#include "alienship.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void AlienShip::initializeGL(GLuint program, float verticalPosition) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_forward = glm::vec2(0.0f, -1.0f);
  m_scale = 0.10f;

  float horizontalPosition =
      Randomizer::getRndNum(DefaultValues::SCREEN_LEFT_LIMIT + m_scale, DefaultValues::SCREEN_RIGHT_LIMIT - m_scale, true);
  m_translation = glm::vec2{horizontalPosition, verticalPosition};

  float horizontalVelocity =
      Randomizer::getRndNum(0.8f, 1.2f, true);
  m_velocity = glm::vec2{horizontalVelocity, 0.0f};

  m_hpBase = 3;
  m_currentLifePoints = m_hpBase;
  m_typeData.m_type = SHIP_TYPE;

  std::array<glm::vec2, 8> positions{
      glm::vec2{-10.0f, +03.0f} /*0*/, glm::vec2{-10.0f, -03.0f} /*1*/,
      glm::vec2{+0.0f, -03.0f} /*2*/,  glm::vec2{+10.0f, -03.0f} /*3*/,
      glm::vec2{+10.0f, +03.0f} /*4*/, glm::vec2{+05.0f, +00.0f} /*5*/,
      glm::vec2{+00.0f, +03.0f} /*6*/, glm::vec2{-05.0f, +00.0f} /*7*/};

  for (auto &position : positions) {
    position /= glm::vec2{-10.0f, -10.0f};
  }

  std::array<int, 18> indices{0, 1, 7, 1, 7, 2, 7, 6, 2, 2, 6, 5, 2, 5, 3, 3, 5, 4};

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

void AlienShip::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  BaseShip::setColor();

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);

  glUseProgram(0);
}

void AlienShip::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void AlienShip::update(float deltaTime) {
  updatePosition(deltaTime);
  updateShooting();
}

void AlienShip::updatePosition(float deltaTime) {
  bool isInInvertRegion = 1 - glm::abs(m_translation.x) < 0.10f;

  if (!m_inInvertRegionFlag && isInInvertRegion) {
    m_velocity *= -1;
    m_inInvertRegionFlag = true;
  } else if (!isInInvertRegion) {
    m_inInvertRegionFlag = false;
  }

  m_translation += m_velocity * deltaTime;
}

void AlienShip::updateShooting() {
  if (m_bulletCoolDownTimer.elapsed() > 250.0 / 1000.0) {
    std::uniform_real_distribution<float> m_randomShoot{0.0f, 1.1f};
    if (Randomizer::getRndNum(0.0f, 1.1f, false) > 0.90f) {
      m_actionData.m_input.set(static_cast<size_t>(Action::Fire));
    } else {
      m_bulletCoolDownTimer.restart();
      m_actionData.m_input.reset(static_cast<size_t>(Action::Fire));
    }
  }
}

void AlienShip::takeDamage() { m_currentLifePoints--; }