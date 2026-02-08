#include "AssetClientMock.h"

using namespace Assets;

void AssetClientMock::EmitImageReady(const QString& name, const QPixmap& pixmap)
{
   emit ImageReady(name, pixmap);
}

void AssetClientMock::EmitFontReady(const QString& name, const QFont& font)
{
   emit FontReady(name, font);
}
