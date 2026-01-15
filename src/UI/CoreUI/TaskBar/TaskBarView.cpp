#include "TaskBarView.h"

#include <TaskBarServiceInterface.h>
#include <ShellWindowInterface.h>

using namespace TaskBar;

TaskBarView::TaskBarView(XPtr<TaskBarServiceInterface> service,
                         ShellWindowInterface* window)
   : QWidget(window)
   , AssetClient("TaskBarView", this)
   , CurrentData()
   , MainLayout(nullptr)
   , StartButton(nullptr)
   , AppIconsLayout(nullptr)
   , MiscIconsLayout(nullptr)
   , DateTimeLayout(nullptr)
{
   CreateUI();
   ConnectToServiceSignals(service);
}

void TaskBarView::HandleViewDataChanged(const TaskBar::ViewData& data)
{
   if(data == CurrentData)
   {
      return;
   }

   if(data.Alignment != CurrentData.Alignment)
   {

   }

   if(data.AssignedMonitor != CurrentData.AssignedMonitor)
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
   MainLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight, this);

   StartButton = new QPushButton(this);
   // const QIcon defaultIcon();
   // StartButton->setIcon()

   setLayout(MainLayout);
   setGeometry({0, 100, 1920, 100});

   AppIconsLayout = new QGridLayout(this);
   MainLayout->addLayout(AppIconsLayout);

   MiscIconsLayout = new QGridLayout(this);
   MainLayout->addLayout(MiscIconsLayout);

   DateTimeLayout = new QVBoxLayout(this);
   MainLayout->addLayout(DateTimeLayout);

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
