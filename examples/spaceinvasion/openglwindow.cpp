#include "openglwindow.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include <vector>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Mouse events
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_player.m_actionData.m_input.set(static_cast<size_t>(Action::Fire));
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT)
      m_player.m_actionData.m_input.reset(static_cast<size_t>(Action::Fire));
  }
  if (event.type == SDL_MOUSEMOTION) {
    glm::ivec2 mousePosition;
    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

    float normalizedMousePosition = (mousePosition.x - 300.0f) / 300.0f;
    m_player.m_lastMousePosition = normalizedMousePosition;

    if (normalizedMousePosition > m_player.m_translation.x) {
      m_player.m_actionData.m_input.reset(static_cast<size_t>(Action::MouseLeft));
      m_player.m_actionData.m_input.set(static_cast<size_t>(Action::MouseRight));
    } else if (normalizedMousePosition < m_player.m_translation.x) {
      m_player.m_actionData.m_input.reset(static_cast<size_t>(Action::MouseRight));
      m_player.m_actionData.m_input.set(static_cast<size_t>(Action::MouseLeft));
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
  m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert",
                                         getAssetsPath() + "stars.frag");
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");

  glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void OpenGLWindow::restart() {
  float AlienDistance = 0.20f;
  m_gameData.m_state = State::Playing;

  m_starLayers.initializeGL(m_starsProgram, 25);
  m_player.initializeGL(m_objectsProgram);
  
  for (auto &enemy : m_aliens) enemy.terminateGL();

  m_aliens.clear();
  int AlienQtt = 5;
  m_aliens.resize(AlienQtt);
  m_bullets.initializeGL(m_objectsProgram);
  float verticalPosition = 0.90f;

  for (auto &enemy : m_aliens) {
    enemy.initializeGL(m_objectsProgram, verticalPosition);
    verticalPosition -= AlienDistance;
  }
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  m_starLayers.update(deltaTime);
  m_player.update(deltaTime);
  m_bullets.update(m_player, m_gameData, deltaTime);
  //m_hpbar.update(deltaTime);


  for (auto &enemy : m_aliens) {
    enemy.update(deltaTime);
    m_bullets.update(enemy, m_gameData, deltaTime);
  }

  if (m_gameData.m_state == State::Playing) {
    m_aliens.remove_if([](const AlienShip &a) { return a.m_currentLifePoints == 0; });
    checkState();
    checkShot();
  }
}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_starLayers.paintGL();
  m_player.paintGL(m_gameData);

  m_bullets.paintGL();
  for (auto &enemy : m_aliens) enemy.paintGL(m_gameData);

}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
  ImVec2 pos(525.0f, 525.0f);

  {
    ImGui::Begin("Vidas");
    ImGui::SetWindowPos(pos);
    ImGui::Text("HP: %i", m_player.m_currentLifePoints);
    ImGui::End();

    auto size{ImVec2(300, 85)};
    auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                         (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
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
  abcg::glDeleteProgram(m_starsProgram);
  glDeleteProgram(m_objectsProgram);
  
  m_player.terminateGL();
  m_bullets.terminateGL();
  
  for (auto &enemy : m_aliens) enemy.terminateGL();
  
  m_starLayers.terminateGL();
}

void OpenGLWindow::checkShot() {
  std::list<MotherShip *> ships = {&m_player};
  for (auto &enemy : m_aliens) ships.push_back(&enemy);

  for (auto &bullet : m_bullets.m_bullets) {
    if (bullet.m_lifeState) continue;
    for (auto &ship : ships) {
      auto distance{glm::distance(bullet.m_translation, ship->m_translation)};
      if (distance < (m_bullets.m_scale + ship->m_scale) * 0.85f &&
          bullet.m_typeData.m_type != ship->m_typeData.m_type &&
          !bullet.m_lifeState) {
        ship->takeDamage();
        bullet.m_lifeState = true;
      }
    }
  }
}

void OpenGLWindow::checkState() {
  if (m_player.m_currentLifePoints == 0) {
    m_gameData.m_state = State::GameOver;
    //m_bullets.terminateGL();
    m_restartWaitTimer.restart(); 
  } else if (m_aliens.empty()) {
    m_gameData.m_state = State::Win;
    //m_bullets.terminateGL();
    m_restartWaitTimer.restart();
  }
}
