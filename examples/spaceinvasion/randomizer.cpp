#include "randomizer.hpp"

float Randomizer::getRndNum(float init, float end, bool requiresNegative) {
  std::uniform_real_distribution<float> m_randomNum{init, end};
  std::default_random_engine m_randomizer;
  
  auto seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomizer.seed(seed);

  float random = m_randomNum(m_randomizer);

  if (requiresNegative) {
    std::uniform_real_distribution<float> m_randomNeg{0, 1};
    float randomNumb = m_randomNeg(m_randomizer);
    float multiplier = randomNumb > 0.5f ? 1 : -1;
    random *= multiplier; 
  }

  return random;
}
