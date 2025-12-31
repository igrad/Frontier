#include "TaskBarView.h"

#include <TaskBarServiceInterface.h>
#include <ShellWindow.h>

using namespace TaskBar;

TaskBarView::TaskBarView(TaskBarServiceInterface* service,
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

void TaskBarView::HandleViewDataChanged(const ViewData& data)
{

}

void CreateUI()
{

}

void TaskBarView::ConnectToServiceSignals(TaskBarServiceInterface* service)
{
   if(nullptr == service)
   {
      LogError("TaskBarService should have been created first!");
      return;
   }

   // Prevents a clang warning about bitwise OR (|) op on these connection types
   // NOLINTNEXTLINE
   const auto conn = static_cast<Qt::ConnectionType>(Qt::UniqueConnection |
                                                     Qt::QueuedConnection);
   connect(service, &TaskBarServiceInterface::ViewDataChanged,
           this, &TaskBarView::HandleViewDataChanged,
           Qt::UniqueConnection);
}
