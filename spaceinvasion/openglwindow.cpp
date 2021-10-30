#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <vector>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_ship.m_actionData.m_input.set(static_cast<size_t>(Action::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_ship.m_actionData.m_input.set(static_cast<size_t>(Action::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_ship.m_actionData.m_input.set(static_cast<size_t>(Action::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_SPACE)
      m_ship.m_actionData.m_input.reset(static_cast<size_t>(Action::Fire));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_ship.m_actionData.m_input.reset(static_cast<size_t>(Action::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_ship.m_actionData.m_input.reset(static_cast<size_t>(Action::Right));
  }
  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_ship.m_actionData.m_input.set(static_cast<size_t>(Action::Fire));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_ship.m_actionData.m_input.reset(static_cast<size_t>(Action::Fire));
  }
  if (event.type == SDL_MOUSEMOTION) {
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    float normalizedMousePosition = (mousePosition.x - 300.0f) / 300.0f;
    m_ship.m_lastMousePosition = normalizedMousePosition;

    if (normalizedMousePosition > m_ship.m_translation.x) {
      m_ship.m_actionData.m_input.reset(static_cast<size_t>(Action::MouseLeft));
      m_ship.m_actionData.m_input.set(static_cast<size_t>(Action::MouseRight));
    } else if (normalizedMousePosition < m_ship.m_translation.x) {
      m_ship.m_actionData.m_input.reset(
          static_cast<size_t>(Action::MouseRight));
      m_ship.m_actionData.m_input.set(static_cast<size_t>(Action::MouseLeft));
    }
  }
}

void OpenGLWindow::initializeGL() {
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);

  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_ship.initializeGL(m_objectsProgram);
  m_bullets.initializeGL(m_objectsProgram);

  m_enemies.clear();
  m_enemies.resize(ENEMY_SHIP_QUANTITY);
  float verticalPosition = ENEMY_SHIP_START_POSITION;

  for (auto &enemy : m_enemies) {
    enemy.initializeGL(m_objectsProgram, verticalPosition);
    verticalPosition -= ENEMY_SHIP_VERTICAL_DELTA;
  }
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  for (auto &enemy : m_enemies) {
    enemy.update(deltaTime);
  }

  m_ship.update(deltaTime);
  m_bullets.update(m_ship, m_gameData, deltaTime);

  for (auto &enemy : m_enemies) {
    m_bullets.update(enemy, m_gameData, deltaTime);
  }

  if (m_gameData.m_state == State::Playing) {
    verifyGameOver();
    verifyShot();
    verifyEnemyLife();
    verifyWin();
  }
}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_bullets.paintGL();
  m_ship.paintGL(m_gameData);

  for (auto &enemy : m_enemies) {
    enemy.paintGL(m_gameData);
  }
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  ImVec2 pos(825.0f, 825.0f);

  {
    ImGui::Begin("Vidas");
    ImGui::SetWindowPos(pos);
    ImGui::Text("HP: %i", m_ship.m_currentLifePoints);
    ImGui::End();

    auto size{ImVec2(300, 85)};
    auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                         (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("You Win!");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_objectsProgram);

  m_bullets.terminateGL();
  m_ship.terminateGL();

  for (auto &enemy : m_enemies) {
    enemy.terminateGL();
  }
}

void OpenGLWindow::verifyShot() {
  std::list<BaseShip *> ships = {&m_ship};
  for (auto &enemy : m_enemies) {
    ships.push_back(&enemy);
  }

  for (auto &bullet : m_bullets.m_bullets) {
    if (bullet.m_dead) continue;

    for (auto &ship : ships) {
      auto distance{glm::distance(bullet.m_translation, ship->m_translation)};
      if (distance < (m_bullets.m_scale + ship->m_scale) * 0.85f &&
          bullet.m_typeData.m_type != ship->m_typeData.m_type &&
          !bullet.m_dead) {
        ship->receiveDamage();
        bullet.m_dead = true;
      }
    }
  }
}

void OpenGLWindow::verifyWin() {
  if (m_enemies.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}

void OpenGLWindow::verifyGameOver() {
  if (m_ship.m_currentLifePoints == 0) {
    m_gameData.m_state = State::GameOver;
    m_restartWaitTimer.restart();
  }
}

void OpenGLWindow::verifyEnemyLife() {
  m_enemies.remove_if(
      [](const EnemyShip &a) { return a.m_currentLifePoints == 0; });
}
