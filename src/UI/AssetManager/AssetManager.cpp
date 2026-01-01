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

void AssetManager::RequestFont(const QString& path, QObject* requester)
{
   QMetaObject::invokeMethod(Loader.Object,
                             "LoadFontAsset",
                             Q_ARG(QString, path));
}

void AssetManager::HandleFontAssetLoaded(const QString& path, const QFont& font)
{

}

void AssetManager::HandleImageAssetLoaded(const QString& path, const QImage& image)
{

}
