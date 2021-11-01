#ifndef ACTIONDATA_HPP_
#define ACTIONDATA_HPP_

#include <bitset>

enum class Action { Fire, MouseLeft, MouseRight };

struct ActionData {
  std::bitset<5> m_input;
};

#endif