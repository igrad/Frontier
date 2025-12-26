#pragma once

#include <Utilities/EnumHelpers.h>

#include <QString>

namespace TaskBar
{
   Q_NAMESPACE

   enum class Alignment: int
   {
      None = 0,
      Bottom,
      Left,
      Top,
      Right
   };
   Q_ENUM_NS(Alignment);

   inline QString ToString(Alignment value)
   {
      return EnumToString(value);
   }

   enum class Directionality: int
   {
      None = 0,
      Left,
      Center,
      Right
   };
   Q_ENUM_NS(Directionality);

   inline QString ToString(Directionality value)
   {
      return EnumToString(value);
   }
}
