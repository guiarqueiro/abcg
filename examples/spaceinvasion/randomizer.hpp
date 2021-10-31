#ifndef RANDOMIZER_HPP_
#define RANDOMIZER_HPP_

#include <random>
#include "abcg.hpp"

class Randomizer {
  public:
    static float getRndNum(float init, float end, bool requiresNegative);    
    
};

#endif