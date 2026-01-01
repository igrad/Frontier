#pragma once

#include <QUuid>

// Creates an AssetId (QUuid) by hashing the str value.
class AssetId: public QUuid
{
public:
   explicit AssetId(const QString& str);
   AssetId() = delete;

private:
   static const QUuid NamespaceId;
};
