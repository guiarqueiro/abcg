#ifndef RANDOMIZER_HPP_
#define RANDOMIZER_HPP_

#include <random>
#include "abcg.hpp"

class Randomizer {
  public:
    static float getRandomNumber(float start, float end, bool isNegativeAcceptable);    
    
};

#endif