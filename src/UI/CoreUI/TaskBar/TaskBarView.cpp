#include "TaskBarView.h"

#include <TaskBarServiceInterface.h>
#include <ShellWindow.h>

using namespace TaskBar;

TaskBarView::TaskBarView(XThread<TaskBarServiceInterface> service,
                         ShellWindow* window)
   : QWidget(window)
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

void CreateUI()
{

}

void TaskBarView::ConnectToServiceSignals(XThread<TaskBarServiceInterface> service)
{
   if(!service.isNull())
   {
      LogError("TaskBarService should have been created first!");
      return;
   }

   connect(service.get(), &TaskBarServiceInterface::ViewDataChanged,
           this, &TaskBarView::HandleViewDataChanged);
}
