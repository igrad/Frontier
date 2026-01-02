#include "AssetId.h"

#include <Log.h>

using namespace Assets;

const QUuid AssetId::NamespaceId = QUuid::createUuid();

AssetId::AssetId(const QString& str)
   : QUuid(AssetId::createUuidV5(NamespaceId, str))
{
   if(str.isEmpty())
   {
      LogWarn("Generating AssetId from empty string is almost always a bad sign!");
   }
}
