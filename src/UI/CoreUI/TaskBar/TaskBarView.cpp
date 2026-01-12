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
}

void TaskBarView::ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service)
{
   if(!service.isNull())
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
