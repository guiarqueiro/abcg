#include "base_ship.hpp"

void BaseShip::setColor() {
  float lifePercentage = 1.0f * m_currentLifePoints / m_hpBase;

  if (lifePercentage > 0.75f) {
    m_color = DefaultValues::GREEN_COLOR;
  } else if (lifePercentage > 0.35f) {
    m_color = DefaultValues::YELLOW_COLOR;
  } else {
    m_color = DefaultValues::RED_COLOR;
  }
}