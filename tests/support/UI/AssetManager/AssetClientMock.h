#pragma once

#include <AssetClientInterface.h>

#include <gmock/gmock.h>
#include <qfont.h>
#include <qpixmap.h>

namespace Assets
{
   class AssetClientMock: public AssetClientInterface
   {
      Q_OBJECT

   public:
      AssetClientMock() = default;
      ~AssetClientMock() = default;

      void EmitImageReady(const QString& name, const QPixmap& pixmap);
      void EmitFontReady(const QString& name, const QFont& font);

      MOCK_METHOD(void,
                  LoadFont,
                  (FontName),
                  (override));
      MOCK_METHOD(void,
                  LoadFont,
                  (const QString&),
                  (override));
      MOCK_METHOD(void,
                  LoadImage,
                  (ImageName),
                  (override));
      MOCK_METHOD(void,
                  LoadImage,
                  (const QString&),
                  (override));
      MOCK_METHOD(bool,
                  IsAssetAvailable,
                  (FontName),
                  (const override));
      MOCK_METHOD(bool,
                  IsAssetAvailable,
                  (ImageName),
                  (const override));
      MOCK_METHOD(bool,
                  IsAssetAvailable,
                  (const QString&),
                  (const override));
      MOCK_METHOD(QFont,
                  GetFont,
                  (FontName),
                  (const override));
      MOCK_METHOD(QFont,
                  GetFont,
                  (const QString&),
                  (const override));
      MOCK_METHOD(QPixmap,
                  GetImage,
                  (ImageName),
                  (const override));
      MOCK_METHOD(QPixmap,
                  GetImage,
                  (const QString&),
                  (const override));
   };
}
