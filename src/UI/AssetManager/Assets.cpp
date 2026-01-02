#include "Assets.h"

#include <Log.h>

using namespace Assets;

QString Assets::ToAssetPath(ImageName name)
{
   static const QMap<ImageName, QString> map =
   {
      { ImageName::_TestName, "_TestName" },
   };

   if(map.contains(name))
   {
      return map.value(name);
   }

   LogWarn(QString("No asset path defined for asset named \"%1\"")
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

   LogWarn(QString("No asset path defined for asset named \"%1\"")
              .arg(ToString(name)));
   return QString();
}
