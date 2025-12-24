#pragma once

#include <Utilities/EnumHelpers.h>

#include <QString>

namespace Wallpaper
{
   Q_NAMESPACE

   enum class Mode: int
   {
      None = 0,
      StaticColor,
      DynamicColor,
      StaticImage,
      Video
   };
   Q_ENUM_NS(Mode);

   inline QString ToString(Mode value)
   {
      return EnumToString(value);
   }

   inline Mode FromString(const QString& str)
   {
      return StringToEnum<Mode>(str);
   }

   enum class Schedule: int
   {
      None = 0,
      Static,
      Sequence,
      Shuffle
   };
   Q_ENUM_NS(Schedule)

   inline QString ToString(Schedule value)
   {
      return EnumToString<Schedule>(value);
   }

   enum class ImageType: int
   {
      None = 0,
      StaticColor,
      DynamicColor,
      Image,
      Video
   };
   Q_ENUM_NS(ImageType)

   inline QString ToString(ImageType type)
   {
      return EnumToString<ImageType>(type);
   }

   enum class DisplayMode: int
   {
      None = 0,
      Fill,
      Fit,
      Stretch,
      Tile,
      Center,
      Span
   };
   Q_ENUM_NS(DisplayMode)

   inline QString ToString(DisplayMode mode)
   {
      return EnumToString<DisplayMode>(mode);
   }

   struct WallpaperData
   {
      ImageType ImageType;
      QList<QColor> Colors;
      QString ImagePath;
      DisplayMode Mode;
      quint8 AssignedMonitor;
   };
}

Q_DECLARE_METATYPE(Wallpaper::WallpaperData);
