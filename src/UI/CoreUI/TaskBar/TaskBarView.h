#pragma once

#include <TaskBarService/TaskBarTypes.h>
#include <TaskBarServiceInterface.h>

#include <AssetClient.h>
#include <Utilities/XPtr.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
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

   private slots:
      void HandleImageReady(Assets::ImageName name, const QPixmap& image);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service);
      void ConnectToAssetClientSignals();

      Assets::AssetClient AssetClient;
      ViewData CurrentData;
      QBoxLayout* MainLayout;
      QPushButton* StartButton;
      QBoxLayout* AppIconsLayout;
      QBoxLayout* MiscIconsLayout;
      QVBoxLayout* DateTimeLayout;

      QTimer AutoHideTimer;
   };
}
