#include "AssetLoader.h"

#include <Log.h>

#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QImage>

using namespace Assets;

AssetLoader::AssetLoader(QObject* parent)
{
   setParent(parent);
}

void AssetLoader::LoadImageAsset(const QString& path)
{
   QFile file(path);

   if(FileExists(path, file) && FileIsReadable(path, file))
   {
      emit ImageAssetLoaded(path, QImage::fromData(file.readAll()));
   }

   file.close();
}

void AssetLoader::LoadFontAsset(const QString& path)
{
   QFile file(path);

   if(!FileExists(path, file) || !FileIsReadable(path, file))
   {
      file.close();
      return;
   }
   file.close();

   const int fontId = QFontDatabase::addApplicationFont(path);
   if(-1 == fontId)
   {
      LogWarn(QString("Failed to load font asset at path: \"%1\"")
                 .arg(path));
      return;
   }

   const QStringList families =
      QFontDatabase::applicationFontFamilies(fontId);

   if(families.isEmpty())
   {
      LogWarn(QString("No font families found in \"%1\" when loading font \"%2\"")
                 .arg(families.join(","), path));
      return;
   }

   emit FontAssetLoaded(path, QFont(families.first()));

   file.close();
}

bool AssetLoader::FileExists(const QString& path, const QFile& file)
{
   if(!file.exists())
   {
      LogWarn(QString("Could not locate file at path: \"%1\"\nError string: \"%2\"")
                 .arg(path,
                      file.errorString()));
      emit FileNotFound(path);
      return false;
   }

   return true;
}

bool AssetLoader::FileIsReadable(const QString& path, QFile& file)
{
   if(!file.open(QIODevice::ReadOnly)) {
      LogWarn(QString("Failed to load image asset at path: \"%1\"\nError string: \"%2\"")
                 .arg(path,
                      file.errorString()));
      emit FailedToLoadAsset(path);

      return false;
   }

   return true;
}
