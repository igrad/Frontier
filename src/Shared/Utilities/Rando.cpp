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

template <typename T>
int Rando::Index(const QList<T>& container, int lowerBound, int upperBound)
{
   if((lowerBound == upperBound) && (0 == upperBound))
   {
      upperBound = container.size();
   }

   if((lowerBound != LowerBound) || (upperBound != UpperBound))
   {
      LowerBound = lowerBound;
      UpperBound = upperBound;

      Dist = std::uniform_int_distribution<>(LowerBound, UpperBound);
   }

   return Dist(Engine);
}
