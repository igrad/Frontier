#include "TaskBarAssetProxy.h"

#include <Assets.h>
#include <AssetClientInterface.h>

#include <QPixmap>

using namespace Assets;
using namespace TaskBar;

TaskBarAssetProxy::TaskBarAssetProxy(QObject* parent)
   : QObject(parent)
   , Client(nullptr)
{
}

void TaskBarAssetProxy::SetAssetClient(AssetClientInterface* assetClient)
{
   Client = assetClient;
   connect(Client, &AssetClientInterface::ImageReady,
           this, &TaskBarAssetProxy::HandleImageReady);
}

void TaskBarAssetProxy::LoadStartButtonImage(const QString& path)
{
   if(nullptr == Client)
   {
      LogError("AssetClientInterface was not set!");
      return;
   }

   const QString imgPath = (path.isEmpty()) ?
                              ToAssetPath(Assets::ImageName::StartMenuImage) :
                              path;
   StartButtonPath = imgPath;

   if(Client->IsAssetAvailable(imgPath))
   {
      emit StartButtonImageReady(Client->GetImage(imgPath));
      return;
   }

   Client->LoadImage(imgPath);
}

void TaskBarAssetProxy::HandleImageReady(const QString& name, const QPixmap& pixmap)
{
   if(name == StartButtonPath)
   {
      emit StartButtonImageReady(pixmap);
   }
}
