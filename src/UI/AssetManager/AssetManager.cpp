#include "AssetManager.h"

AssetManager* AssetManager::Instance = nullptr;

AssetManager* AssetManager::GetInstance()
{
   return Instance;
}

AssetManager::AssetManager(XPtr<AssetLoaderInterface> loader,
                           QObject* parent)
   : QObject(parent)
   , Loader(loader)
   , PixmapCache()
   , InFlight()
{
   Instance = this;

   connect(Loader.get(), &AssetLoaderInterface::FontAssetLoaded,
           this, &AssetManager::HandleFontAssetLoaded);
   connect(Loader.get(), &AssetLoaderInterface::ImageAssetLoaded,
           this, &AssetManager::HandleImageAssetLoaded);
}

AssetManager::~AssetManager()
{

}

AssetId AssetManager::RequestFont(const QString& path, QObject* requester)
{
   const AssetId id(path);
   if(IsInFlight(id))
   {
      return id;
   }

   QMetaObject::invokeMethod(Loader.Object,
                             "LoadFontAsset",
                             Q_ARG(AssetId, id),
                             Q_ARG(QString, path));

   return id;
}

AssetId AssetManager::RequestImage(const QString& path, QObject* requester)
{
   const AssetId id(path);
   if(IsInImageCache(id))
   {
      return id;
   }
   else if(IsInFlight(id))
   {
      return id;
   }

   QMetaObject::invokeMethod(Loader.Object,
                             "LoadImageAsset",
                             Q_ARG(AssetId, id),
                             Q_ARG(QString, path));

   return id;
}

void AssetManager::HandleFontAssetLoaded(const AssetId& id, const QFont& font)
{

}

void AssetManager::HandleImageAssetLoaded(const AssetId& id, const QImage& image)
{

}

bool AssetManager::IsInImageCache(const AssetId& id) const
{
   return PixmapCache.contains(id);
}

bool AssetManager::IsInFlight(const AssetId& id) const
{
   return InFlight.contains(id);
}
