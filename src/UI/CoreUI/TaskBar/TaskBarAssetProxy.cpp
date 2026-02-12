#include "TaskBarAssetProxy.h"

#include <Assets.h>
#include <AssetClientInterface.h>

#include <QPixmap>

using namespace Assets;
using namespace TaskBar;

TaskBarAssetProxy::TaskBarAssetProxy(AssetClientInterface& assetClient,
                                     QObject* parent)
   : QObject(parent)
   , Client(assetClient)
{
}

void TaskBarAssetProxy::LoadStartButtonImage(const QString& path)
{
   const QString imgPath = (path.isEmpty()) ?
                              ToAssetPath(Assets::ImageName::StartMenuImage) :
                              path;
   StartButtonPath = imgPath;

   if(Client.IsAssetAvailable(imgPath))
   {
      emit StartButtonImageReady(Client.GetImage(imgPath));
      return;
   }

   Client.LoadImage(imgPath);
}

void TaskBarAssetProxy::HandleImageReady(const QString& name, const QPixmap& pixmap)
{
   if(name == StartButtonPath)
   {
      emit StartButtonImageReady(pixmap);
   }
}
