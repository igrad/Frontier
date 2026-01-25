#pragma once

#include "TaskBarAssetProxy.h"
#include <TaskBarServiceInterface.h>

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
      void HandleInitialViewData(const DisplayID& displayID, const TaskBar::ViewData& data);
      void HandleStartButtonImageReady(const QPixmap& pix);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service);
      void ConnectToAssetProxy();
      void SetOrientation(Orientation orientation);
      void SetStartButtonImagePath(const QString& path);

      DisplayID Display;
      DisplayInfo Info;
      bool InitialDisplaySettingsReceived;
      TaskBarAssetProxy AssetProxy;
      TaskBar::ViewData CurrentData;

      QBoxLayout* MainLayout;
      QPushButton* StartButton;
      QGridLayout* AppIconsLayout;
      QGridLayout* MiscIconsLayout;
      QVBoxLayout* DateTimeLayout;

      QTimer AutoHideTimer;
   };
}
