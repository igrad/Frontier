#include "AssetClient.h"
#include "AssetManager.h"

#include <Log.h>

#include <QFont>

using namespace Assets;

AssetClient::AssetClient(QObject* parent)
   : Manager(AssetManager::GetInstance())
   , LoadingImages()
   , LoadingFonts()
{
   setParent(parent);
   ConnectToAssetManager();
}

void AssetClient::LoadFont(FontName name)
{
   LoadFont(ToAssetPath(name));
}

void AssetClient::LoadFont(const QString& path)
{
   if(IsAssetAvailable(path))
   {
      emit FontReady(path, GetFont(path));
   }
   else
   {
      Manager->RequestFont(path);
      LoadingFonts.insert(path);
   }
}

void AssetClient::LoadImage(ImageName name)
{
   LoadImage(ToAssetPath(name));
}

void AssetClient::LoadImage(const QString& path)
{
   if(IsAssetAvailable(path))
   {
      emit ImageReady(path, Manager->GetImage(path));
   }
   else
   {
      Manager->RequestImage(path);
      LoadingImages.insert(path);
   }
}

bool AssetClient::IsAssetAvailable(FontName name) const
{
   return IsAssetAvailable(ToAssetPath(name));
}

bool AssetClient::IsAssetAvailable(ImageName name) const
{
   return IsAssetAvailable(ToAssetPath(name));
}

bool AssetClient::IsAssetAvailable(const QString& path) const
{
   return Manager-IsAssetAvailable(path);
}

QFont AssetClient::GetFont(FontName name) const
{
   return GetFont(ToAssetPath(name));
}

QFont AssetClient::GetFont(const QString& path) const
{
   return Manager->GetFont(path);
}

QPixmap AssetClient::GetImage(ImageName name) const
{
   return GetImage(ToAssetPath(name));
}

QPixmap AssetClient::GetImage(const QString& path) const
{
   return Manager->GetImage(path);
}

void AssetClient::HandleFontLoaded(const QString& path, const QFont& font)
{
   LoadingFonts.remove(path);

   emit FontReady(path, font);
}

void AssetClient::HandleImageLoaded(const QString& path, const QPixmap& pixmap)
{
   LoadingImages.remove(path);

   emit ImageReady(path, pixmap);
}

void AssetClient::ConnectToAssetManager()
{
   connect(Manager, &AssetManager::FontLoaded,
           this, &AssetClient::HandleFontLoaded);
   connect(Manager, &AssetManager::ImageLoaded,
           this, &AssetClient::HandleImageLoaded);
}
