#pragma once

#include <Utilities/EnumHelpers.h>

#include <QObject>

#include <climits>

namespace Assets
{
   Q_NAMESPACE

   enum class ImageName: int
   {
      None = 0,
      _TestName = INT_MAX
   };
   Q_ENUM_NS(ImageName)

   // Get the text representation of the enum value
   inline QString ToString(ImageName name)
   {
      return EnumToString<ImageName>(name);
   }

   enum class FontName: int
   {
      None = 0,
      _TestName = INT_MAX
   };
   Q_ENUM_NS(FontName)

   // Get the text representation of the enum value
   inline QString ToString(FontName name)
   {
      return EnumToString<FontName>(name);
   }

   // Get the file path or "url" for the asset file in relations to the assets folder
   QString ToAssetPath(ImageName name);

   // Get the file path or "url" for the asset file in relations to the assets folder
   QString ToAssetPath(FontName name);
}

Q_DECLARE_METATYPE(Assets::FontName)
Q_DECLARE_METATYPE(Assets::ImageName)
