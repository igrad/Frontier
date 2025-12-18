#pragma once

#include <Log.h>

#include <QString>
#include <QStringList>
#include <QVariant>

namespace Settings
{
   constexpr const char* CONNECTION_NAME = "SettingsService";

   inline const std::string ToSettingString(const QVariant& val)
   {
      QString str("\"::" + val.toString() + "::\"");
      return str.toStdString();
   };

   inline const QVariant FromSettingString(const QString& val)
   {
      QStringList tokens = val.split("::");
      if(tokens.size() != 3)
      {
         LogError(QString("Failed to properly tokenize setting string: \"%1\"")
           .arg(val));
         return "";
      }

      return tokens[1];
   }
}
