#include <iostream>
#include <limits>
#include <random>
#include <sstream>

struct Range
{
  int min{};
  int max{};
};


template<class Out = std::ostream> void out_random_data(Range r, int times = 1, Out &out = std::cout)
{
  using Seed = std::random_device;
  using Engine = std::default_random_engine;
  // supported int Types // check https://en.cppreference.com/w/cpp/header/random
  // short, int, long, long long,
  // unsigned short, unsigned int, unsigned long, or unsigned long long
  using intType = unsigned;
  using Distribution = std::uniform_int_distribution<intType>;

  Seed seed;
  Engine engine{ seed() };

  Distribution t_distribution(r.min, r.max);


  auto generate_t = [&]() { return t_distribution(engine); };

  using std::endl;

  for (int i = 0; i < times; i++) { out << generate_t() << " "; }
  out << endl;
}
