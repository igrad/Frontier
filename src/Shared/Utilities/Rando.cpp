#include "Rando.h"

Rando::Rando()
   : Device(std::random_device())
   , Engine(Device())
   , LowerBound(0)
   , UpperBound(0)
   , Dist()
{
}

Rando::Rando(int lowerBound, int upperBound)
   : Device(std::random_device())
   , Engine(Device())
   , LowerBound(lowerBound)
   , UpperBound(upperBound)
   , Dist(LowerBound, UpperBound)
{
   assert(LowerBound <= UpperBound && "Lower bound of Rando must be <= upper bound");
}

Rando::Rando(const Rando& other)
   : Device()
   , Engine(Device())
   , LowerBound(other.LowerBound)
   , UpperBound(other.UpperBound)
   , Dist(other.Dist)
{
}

int Rando::Value()
{
   return Dist(Engine);
}

int Rando::Value(int lowerBound, int upperBound)
{
   LowerBound = lowerBound;
   UpperBound = upperBound;

   assert(LowerBound <= UpperBound && "Lower bound of Rando must be <= upper bound");

   Dist = std::uniform_int_distribution<>(LowerBound, UpperBound);

   return Dist(Engine);
};
