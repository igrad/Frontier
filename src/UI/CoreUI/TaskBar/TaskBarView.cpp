#include "TaskBarView.h"

#include <TaskBarServiceInterface.h>
#include <ShellWindowInterface.h>

using namespace TaskBar;

TaskBarView::TaskBarView(XPtr<TaskBarServiceInterface> service,
                         ShellWindowInterface* window,
                         const DisplayInfo& info)
   : QWidget(window)
   , Display(info.ID)
   , Info(info)
   , AssetClient("TaskBarView", this)
   , CurrentData()
{
   CreateUI();
   ConnectToServiceSignals(service);
}

void TaskBarView::HandleViewDataChanged(const DisplayID& displayID, const TaskBar::ViewData& data)
{
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

   }

   if(data.Opacity != CurrentData.Opacity)
   {

   }

   CurrentData = data;
}

void TaskBarView::HandleImageReady(Assets::ImageName name, const QPixmap& image)
{
   switch(name)
   {
   default:
      break;
   }
}

void TaskBarView::CreateUI()
{
   MainLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);

   StartButton = new QPushButton(this);
   // const QIcon defaultIcon();
   // StartButton->setIcon()

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

void TaskBarView::ConnectToAssetClientSignals()
{
   connect(&AssetClient, &Assets::AssetClient::ImageReady,
           this, &TaskBarView::HandleImageReady);
}
