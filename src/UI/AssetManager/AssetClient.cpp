#include "AssetClient.h"
#include "AssetManager.h"

#include <Log.h>

#include <QFont>

using namespace Assets;

AssetClient::AssetClient(const QString& owner, QObject* parent)
   : QObject(parent)
   , Manager(AssetManager::GetInstance())
   , Owner(owner)
   , BatchLoadNames()
   , LoadingImages()
   , LoadingFonts()
{
   ConnectToAssetManager();
}

void AssetClient::BatchLoadImages(const QList<ImageName>& names)
{
   for(ImageName name : names)
   {
      if(!BatchLoadNames.contains(name))
      {
         BatchLoadNames.insert(name);
      }

      PrivateLoadImage(name, true);
   }
}

void AssetClient::LoadFont(FontName name)
{
   if(IsAssetAvailable(name))
   {
      emit FontReady(name, GetFont(name));
      return;
   }

   const QString path = ToAssetPath(name);
   Manager->RequestFont(path);
   LoadingFonts[path] = name;
}

void AssetClient::LoadImage(ImageName name)
{
   PrivateLoadImage(name, false);
}

bool AssetClient::IsAssetAvailable(FontName name) const
{
   return Manager->IsAssetAvailable(ToAssetPath(name));
}

bool AssetClient::IsAssetAvailable(ImageName name) const
{
   return Manager->IsAssetAvailable(ToAssetPath(name));
}

QFont AssetClient::GetFont(FontName name) const
{
   return Manager->GetFont(ToAssetPath(name));
}

QPixmap AssetClient::GetImage(ImageName name) const
{
   return Manager->GetImage(ToAssetPath(name));
}

void AssetClient::HandleFontLoaded(const QString& path, const QFont& font)
{
   const FontName name = LoadingFonts[path];

   LoadingFonts.remove(path);

   emit FontReady(name, font);
}

void AssetClient::HandleImageLoaded(const QString& path, const QPixmap& pixmap)
{
   const ImageName name = LoadingImages[path];
   if(BatchLoadNames.contains(name))
   {
      BatchLoadInProgress[name] = pixmap;
      CheckBatchReady();
   }
   else
   {
      LoadingImages.remove(path);

      emit ImageReady(name, pixmap);
   }
}

void AssetClient::PrivateLoadImage(ImageName name, bool batch)
{
   const QString path = ToAssetPath(name);

   if(IsAssetAvailable(name))
   {
      BatchLoadInProgress[name] = Manager->GetImage(path);
      BatchLoadNames.remove(name);
      LoadingImages.remove(path);

      if(batch)
      {
         CheckBatchReady();
      }
   }
   else
   {
      BatchLoadNames.insert(name);
      Manager->RequestImage(ToAssetPath(name));
      LoadingImages[path] = name;
   }
}

void AssetClient::CheckBatchReady()
{
   bool ready = true;
   for(const ImageName name : std::as_const(BatchLoadNames))
   {
      if(!BatchLoadInProgress.contains(name))
      {
         ready = false;
         break;
      }
   }

   if(ready)
   {
      LogInfo(QString("Batch load for client \"%1\" is ready")
                 .arg(Owner));

      emit BatchLoadImagesReady(BatchLoadInProgress);

      BatchLoadNames.clear();
      BatchLoadInProgress.clear();
   }
}

void AssetClient::ConnectToAssetManager()
{
   connect(Manager, &AssetManager::FontLoaded,
           this, &AssetClient::HandleFontLoaded);
   connect(Manager, &AssetManager::ImageLoaded,
           this, &AssetClient::HandleImageLoaded);
}
