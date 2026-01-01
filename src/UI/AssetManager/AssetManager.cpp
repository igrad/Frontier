#include "AssetManager.h"

#include <Utilities/MethodLookupHelpers.h>
#include <Log.h>

#include <QFont>
#include <QPixmap>

namespace
{
   constexpr const char* const IMAGE_HANDLER_FUNCTION_SIG = "HandleImageLoaded(const QPixmap&)";
   constexpr const char* const FONT_HANDLER_FUNCTION_SIG = "HandleFontLoaded(const QFont&)";
}

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

   bool invoke = true;
   if(nullptr == requester)
   {
      LogError("A nullptr cannot request an image");
      invoke = false;
   }
   else if(!QObjectHasMethodDeclared(requester, IMAGE_HANDLER_FUNCTION_SIG))
   {
      LogError(QString("Image requester \"%1\" does not have \"%2\" method!")
                  .arg(requester->objectName(), IMAGE_HANDLER_FUNCTION_SIG));
      invoke = false;
   }
   else if(IsInImageCache(id))
   {
      invoke = false;
   }
   else if(IsInFlight(id))
   {
      invoke = false;
   }

   if(invoke)
   {
      QMetaObject::invokeMethod(Loader.Object,
                                "LoadImageAsset",
                                Q_ARG(AssetId, id),
                                Q_ARG(QString, path));
      InFlight[id] = path;
   }

   return id;
}

void AssetManager::HandleFontAssetLoaded(const AssetId& id, const QFont& font)
{
   const QString str = InFlight[id];
   InFlight.remove(id);

   LogInfo(QString("Loaded font \"%1\"").arg(str));

   emit FontLoaded(id, font);
}

void AssetManager::HandleImageAssetLoaded(const AssetId& id, const QImage& image)
{
   const QString str = InFlight[id];
   InFlight.remove(id);

   const QPixmap pix = QPixmap::fromImage(image);
   PixmapCache[id] = QPair<QString, QPixmap>(str, pix);

   LogInfo(QString("Loaded image \"%1\"").arg(id.toString()));

   emit ImageLoaded(id, pix);
}

bool AssetManager::IsInImageCache(const AssetId& id) const
{
   return PixmapCache.contains(id);
}

bool AssetManager::IsInFlight(const AssetId& id) const
{
   return InFlight.contains(id);
}
