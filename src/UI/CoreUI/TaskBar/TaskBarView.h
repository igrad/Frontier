#pragma once

#include <TaskBarService/TaskBarTypes.h>
#include <TaskBarServiceInterface.h>

#include <AssetClient.h>
#include <SettingsClient.h>
#include <Utilities/XPtr.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class ShellWindowInterface;

namespace TaskBar
{
   class TaskBarView: public QWidget
   {
      Q_OBJECT

   public:
      TaskBarView(XPtr<TaskBarServiceInterface> service,
                  ShellWindowInterface* window,
                  const DisplayInfo& displayInfo);
      ~TaskBarView() = default;

   public:
      void HandleViewDataChanged(const DisplayID& displayID, const TaskBar::ViewData& data);

   private slots:
      void HandleImageReady(Assets::ImageName name, const QPixmap& image);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service);
      void ConnectToAssetClientSignals();

      DisplayID Display;
      DisplayInfo Info;
      Assets::AssetClient AssetClient;
      TaskBar::ViewData CurrentData;
      QBoxLayout* MainLayout;
      QPushButton* StartButton;
      QGridLayout* AppIconsLayout;
      QGridLayout* MiscIconsLayout;
      QVBoxLayout* DateTimeLayout;

      QTimer AutoHideTimer;
   };
}
