#ifndef TYPEDATA_HPP_
#define TYPEDATA_HPP_

#include <bitset>

enum class Type { Alien, Player };

struct TypeData {
  Type m_type{Type::Player};
};

#endif