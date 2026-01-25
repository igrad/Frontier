#include "Assets.h"

#include <Log.h>

using namespace Assets;

QString Assets::ToAssetPath(ImageName name)
{
   static const QMap<ImageName, QString> map =
   {
      { ImageName::StartMenuImage, "://images/icons/StartMenuImage.png" },
      { ImageName::_TestName, "_TestName" },
   };

   if(map.contains(name))
   {
      return map.value(name);
   }

   LogError(QString("No asset path defined for asset named \"%1\". Define in Assets.cpp")
               .arg(ToString(name)));
   return QString();
}

QString Assets::ToAssetPath(FontName name)
{
   static const QMap<FontName, QString> map =
   {
      { FontName::_TestName, "_TestName" },
   };

   if(map.contains(name))
   {
      return map.value(name);
   }

   LogWarn(QString("No asset path defined for asset named \"%1\". Define in Assets.cpp")
              .arg(ToString(name)));
   return QString();
}
