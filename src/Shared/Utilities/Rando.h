#pragma once

#include <QIterable>

#include <random>

class Rando
{
public:
   Rando();
   Rando(int lowerBound, int upperBound);

   template <typename T>
   explicit Rando(const QIterable<T>& container);

   int Value();
   int Value(int lowerBound, int upperBound);

   template <typename T>
   int Index(const QList<T>& container,
             int lowerBound = 0,
             int upperBound = 0);

private:
   std::random_device Device;
   std::mt19937 Engine;
   int LowerBound;
   int UpperBound;
   std::uniform_int_distribution<> Dist;
};
