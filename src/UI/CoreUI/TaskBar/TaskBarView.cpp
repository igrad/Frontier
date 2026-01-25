#include "TaskBarView.h"

#include <TaskBarServiceInterface.h>
#include <ShellWindowInterface.h>

#include <QGraphicsOpacityEffect>

using namespace TaskBar;

TaskBarView::TaskBarView(XPtr<TaskBarServiceInterface> service,
                         ShellWindowInterface* window,
                         const DisplayInfo& info)
   : QWidget(window)
   , Display(info.ID)
   , Info(info)
   , CurrentData()
   , InitialDisplaySettingsReceived(false)
{
   CreateUI();
   ConnectToServiceSignals(service);
   ConnectToAssetProxy();
}

void TaskBarView::HandleViewDataChanged(const DisplayID& displayID, const TaskBar::ViewData& data)
{
   if(!InitialDisplaySettingsReceived)
   {
      InitialDisplaySettingsReceived = true;
      HandleInitialViewData(displayID, data);

      CurrentData = data;
      return;
   }

   if(data == CurrentData)
   {
      return;
   }

   if(data.Alignment != CurrentData.Alignment)
   {

   }

   if(data.DisplayID != CurrentData.DisplayID)
   {
      // TODO: Make sure we're processing the settings update for the correct monitor!
   }

   if(data.AutoHide != CurrentData.AutoHide)
   {

   }

   if(data.AutoHideDelayMs != CurrentData.AutoHideDelayMs)
   {

   }

   if(data.Orientation != CurrentData.Orientation)
   {
      SetOrientation(data.Orientation);
   }

   if(data.Opacity != CurrentData.Opacity)
   {
      QGraphicsOpacityEffect* effect = nullptr;
      if(data.Opacity != 100)
      {
         effect = new QGraphicsOpacityEffect(this);
         effect->setOpacity(data.Opacity / 100);
      }

      setGraphicsEffect(effect);
   }

   if(data.StartButtonShown != CurrentData.StartButtonShown)
   {
      StartButton->show();
   }

   if(data.StartButtonImagePath != CurrentData.StartButtonImagePath)
   {
      SetStartButtonImagePath(data.StartButtonImagePath);
   }

   CurrentData = data;
}

void TaskBarView::HandleInitialViewData(const DisplayID& displayID, const TaskBar::ViewData& data)
{
   // Just set each member with no regard to differences. This ensures that if the field is
   // blank, then we initialize with default settings and assets.
   SetStartButtonImagePath(data.StartButtonImagePath);
}

void TaskBarView::HandleStartButtonImageReady(const QPixmap& pix)
{
   const QIcon icon(pix);
   StartButton->setIcon(icon);
   const QList<QSize> sizes = icon.availableSizes();
   StartButton->setIconSize(sizes[0]); // WIP
}

void TaskBarView::CreateUI()
{
   MainLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);

   StartButton = new QPushButton();
   // const QIcon defaultIcon();
   // StartButton->setIcon()
   MainLayout->addWidget(StartButton);

   // NOTE: Just for dev to get something on screen. Should be handled initially by DisplayInfo
   // and then updated by settings
   setGeometry({0, 100, 1920, 100});

   AppIconsLayout = new QGridLayout();
   MainLayout->addLayout(AppIconsLayout);

   MiscIconsLayout = new QGridLayout();
   MainLayout->addLayout(MiscIconsLayout);

   DateTimeLayout = new QVBoxLayout();
   MainLayout->addLayout(DateTimeLayout);

   setLayout(MainLayout);
   show();
}

void TaskBarView::ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service)
{
   if(service.isNull())
   {
      LogError("TaskBarService should have been created first!");
      return;
   }

   connect(service.get(), &TaskBarServiceInterface::ViewDataChanged,
           this, &TaskBarView::HandleViewDataChanged);
}

void TaskBarView::ConnectToAssetProxy()
{
   connect(&AssetProxy, &TaskBarAssetProxy::StartButtonImageReady,
           this, &TaskBarView::HandleStartButtonImageReady);
}

void TaskBarView::SetOrientation(Orientation orientation)
{
   switch(orientation)
   {
   case Orientation::LeftToRight:
      MainLayout->setDirection(QBoxLayout::LeftToRight);
      break;
   case Orientation::RightToLeft:
      MainLayout->setDirection(QVBoxLayout::RightToLeft);
      break;
   }
}

void TaskBarView::SetStartButtonImagePath(const QString& path)
{
   if(path.isEmpty())
   {
      AssetProxy.LoadStartButtonImage(path);
   }
}
