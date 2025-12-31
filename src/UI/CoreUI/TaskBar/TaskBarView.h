#pragma once

#include <TaskBarService/TaskBarTypes.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class ShellWindow;

namespace TaskBar
{
   class TaskBarServiceInterface;

   class TaskBarView: public QWidget
   {
      Q_OBJECT

   public:
      TaskBarView(TaskBarServiceInterface* service,
                  ShellWindow* window);
      ~TaskBarView() = default;

   public:
      void HandleViewDataChanged(const ViewData& data);

   private:
      void CreateUI();
      void ConnectToServiceSignals(TaskBarServiceInterface* service);

      ViewData CurrentData;
      QBoxLayout* MainLayout;
      QWidget* StartButton;
      QBoxLayout* AppIconsLayout;
      QBoxLayout* MiscIconsLayout;
      QVBoxLayout* DateTimeLayout;

      QTimer AutoHideTimer;
   };
}
