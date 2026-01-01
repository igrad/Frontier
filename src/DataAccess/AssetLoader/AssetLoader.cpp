#include "AssetLoader.h"

#include <Log.h>

#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QImage>

AssetLoader::AssetLoader(QObject* parent)
{
   setParent(parent);
}

void AssetLoader::LoadImageAsset(const QString& path)
{
   QFile f(path);

   if(!f.open(QIODevice::ReadOnly))
   {
      LogWarn(QString("Failed to load image asset at path: \"%1\"")
                 .arg(path));
      return;
   }

   emit ImageAssetLoaded(path, QImage(f.readAll()));
}

void AssetLoader::LoadFontAsset(const QString& path)
{
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
}
