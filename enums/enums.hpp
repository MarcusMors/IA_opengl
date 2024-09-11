#ifndef __ENUMS_H__
#define __ENUMS_H__

#include <iostream>


enum Color {
  red,
  blue,
  green,
  none,
};

std::ostream &operator<<(std::ostream &os, Color c)
{
  switch (c) {
  case Color::red: os << "red"; break;
  case Color::blue: os << "blue"; break;
  case Color::green: os << "green"; break;
  case Color::none: os << "none"; break;
  default: break;
  }
  return os;
}

#endif// __ENUMS_H__