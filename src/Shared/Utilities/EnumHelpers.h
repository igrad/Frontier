#pragma once

#include <QString>

#include <QMetaEnum>

template <typename T>
QString EnumToString(T value)
{
   QMetaEnum metaEnum = QMetaEnum::fromType<T>();

   return QString(metaEnum.valueToKey(static_cast<int>(value)));
}

template <typename T>
T StringToEnum(const QString& str)
{
   const QMetaEnum metaEnum = QMetaEnum::fromType<T>();

   const int value = metaEnum.keyToValue(str.toUtf8().constData());
   return static_cast<T>(value);
}
