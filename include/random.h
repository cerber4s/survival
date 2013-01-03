#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>
#include <ctime>

class Random
{
public:
  static void Initialize();
  
  static double Double();
  static double Double(double max);
  static double Double(double min, double max);
  
  static int Int(int max);
  
private:  
  Random();
  virtual ~Random();
  
};

Random::Random()
{
  
}

Random::~Random()
{
}

void Random::Initialize() 
{
  std::srand(std::time(0));
}

int Random::Int(int max)
{
  return std::rand() % max;
}

double Random::Double()
{
  return Double(0.0, 1.0);
}

double Random::Double(double max)
{
  return Double(0.0, max);
}

double Random::Double(double min, double max)
{
  return min + (((max - min) / static_cast<double>(RAND_MAX)) * std::rand());
}

#endif //RANDOM_H