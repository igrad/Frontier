#pragma once

#include <Log.h>

#include <QString>

#include <QMetaEnum>

template <typename T>
QString EnumToString(T value)
{
   const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
   return QString(metaEnum.valueToKey(static_cast<int>(value)));
}

template <typename T>
T StringToEnum(const QString& str)
{
   const QMetaEnum metaEnum = QMetaEnum::fromType<T>();
   const QByteArray data = str.toUtf8().constData();

   bool ok = true;
   const int raw = metaEnum.keyToValue(data, &ok);

   if(ok)
   {
      return static_cast<T>(raw);
   }
   else
   {
      LogWarn(QString("Could not conver QString to enum type: %1 with value %2")
                 .arg(metaEnum.name(), raw));
      return static_cast<T>(0);
   }
}
