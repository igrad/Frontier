#include "AssetManager.h"
#include "AssetClient.h"

#include <Utilities/MethodLookupHelpers.h>
#include <Log.h>

#include <QFont>
#include <QFontDatabase>
#include <QPixmap>

using namespace Assets;

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
   , FontCache()
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

bool AssetManager::IsAssetAvailable(const QString& path) const
{
   return PixmapCache.contains(path) || FontCache.contains(path);
}

QFont AssetManager::GetFont(const QString& path) const
{
   if(IsInFontCache(path))
   {
      return QFont(FontCache[path]);
   }

   LogWarn("GetFont used before the font was loaded!");
   return QFont();
}

QPixmap AssetManager::GetImage(const QString& path) const
{
   if(IsInImageCache(path))
   {
      return PixmapCache[path].second;
   }

   LogWarn("GetImage used before the image was loaded!");

   return QPixmap();
}

void AssetManager::RequestFont(const QString& path, AssetClient* requester)
{
   bool invoke = true;
   if(FontCache.contains(path))
   {
      invoke = false;
   }
   else if(IsInFlight(path))
   {
      invoke = false;
   }

   if(invoke)
   {
      QMetaObject::invokeMethod(Loader.Object,
                                "LoadFontAsset",
                                Q_ARG(QString, path));
   }
}

void AssetManager::RequestImage(const QString& path, AssetClient* requester)
{
   bool invoke = true;
   if(nullptr == requester)
   {
      LogError("A nullptr cannot request an image");
      invoke = false;
   }
   else if(IsInImageCache(path))
   {
      invoke = false;
   }
   else if(IsInFlight(path))
   {
      invoke = false;
   }

   if(invoke)
   {
      QMetaObject::invokeMethod(Loader.Object,
                                "LoadImageAsset",
                                Q_ARG(QString, path));
      InFlight[path] = path;
   }
}

void AssetManager::HandleFontAssetLoaded(const QString& path, const QFont& font)
{
   const QString str = InFlight[path];
   InFlight.remove(path);

   LogInfo(QString("Loaded font \"%1\"").arg(str));

   FontCache[path] = font.family();

   emit FontLoaded(path, font);
}

void AssetManager::HandleImageAssetLoaded(const QString& path, const QImage& image)
{
   const QString str = InFlight[path];
   InFlight.remove(path);

   const QPixmap pix = QPixmap::fromImage(image);
   PixmapCache[path] = QPair<QString, QPixmap>(str, pix);

   LogInfo(QString("Loaded image \"%1\"").arg(path));

   emit ImageLoaded(path, pix);
}

bool AssetManager::IsInFontCache(const QString& path) const
{
   return FontCache.contains(path);
}

bool AssetManager::IsInImageCache(const QString& path) const
{
   return PixmapCache.contains(path);
}

bool AssetManager::IsInFlight(const QString& path) const
{
   return InFlight.contains(path);
}

bool AssetManager::IsFontLoaded(const QString& path) const
{
   const QStringList families = QFontDatabase::families();
   return !families.isEmpty();
}
