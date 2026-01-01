#pragma once

#include <TaskBarService/TaskBarTypes.h>
#include <TaskBarServiceInterface.h>

#include <Utilities/XPtr.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class ShellWindow;

namespace TaskBar
{
   class TaskBarView: public QWidget
   {
      Q_OBJECT

   public:
      TaskBarView(XPtr<TaskBarServiceInterface> service,
                  ShellWindow* window);
      ~TaskBarView() = default;

   public:
      void HandleViewDataChanged(const TaskBar::ViewData& data);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service);

      ViewData CurrentData;
      QBoxLayout* MainLayout;
      QWidget* StartButton;
      QBoxLayout* AppIconsLayout;
      QBoxLayout* MiscIconsLayout;
      QVBoxLayout* DateTimeLayout;

      QTimer AutoHideTimer;
   };
}
