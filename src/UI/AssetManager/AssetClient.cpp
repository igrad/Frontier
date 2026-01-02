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

   const AssetId id = Manager->RequestFont(ToAssetPath(name), this);
   LoadingFonts[id] = name;
}

void AssetClient::LoadImage(ImageName name)
{
   PrivateLoadImage(name, false);
}

bool AssetClient::IsAssetAvailable(FontName name) const
{
   return Manager->IsAssetAvailable(AssetId(ToAssetPath(name)));
}

bool AssetClient::IsAssetAvailable(ImageName name) const
{
   return Manager->IsAssetAvailable(AssetId(ToAssetPath(name)));
}

QFont AssetClient::GetFont(FontName name) const
{
   return Manager->GetFont(AssetId(ToAssetPath(name)));
}

QPixmap AssetClient::GetImage(ImageName name) const
{
   return Manager->GetImage(AssetId(ToAssetPath(name)));
}

void AssetClient::HandleFontLoaded(const AssetId& id, const QFont& font)
{
   const FontName name = LoadingFonts[id];

   LoadingFonts.remove(id);

   emit FontReady(name, font);
}

void AssetClient::HandleImageLoaded(const AssetId& id, const QPixmap& pixmap)
{
   const ImageName name = LoadingImages[id];
   if(BatchLoadNames.contains(name))
   {
      BatchLoadInProgress[name] = pixmap;
      CheckBatchReady();
   }
   else
   {
      LoadingImages.remove(id);

      emit ImageReady(name, pixmap);
   }
}

void AssetClient::PrivateLoadImage(ImageName name, bool batch)
{
   if(IsAssetAvailable(name))
   {
      const AssetId id(ToAssetPath(name));
      BatchLoadInProgress[name] = Manager->GetImage(id);
      BatchLoadNames.remove(name);
      LoadingImages.remove(id);

      if(batch)
      {
         CheckBatchReady();
      }
   }
   else
   {
      BatchLoadNames.insert(name);
      const AssetId id = Manager->RequestImage(ToAssetPath(name), this);
      LoadingImages[id] = name;
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
