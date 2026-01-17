#pragma once

#include <QList>

#include <random>

class Rando
{
public:
   Rando();
   Rando(int lowerBound, int upperBound);
   Rando(const Rando& other);

   int Value();
   int Value(int lowerBound, int upperBound);

   template <typename T>
   int Index(const QList<T>& container, int lowerBound = 0, int upperBound = 0)
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

private:
   std::random_device Device;
   std::mt19937 Engine;
   int LowerBound;
   int UpperBound;
   std::uniform_int_distribution<> Dist;
};
