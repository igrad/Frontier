#pragma once

#include "TaskBarAssetProxy.h"
#include <TaskBarServiceInterface.h>

#include <Utilities/XPtr.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>
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
      void SetLayout(const TaskBar::ViewData& data);
      void SetDirection(Direction direction);
      void SetStartButtonImagePath(const QString& path);
      void SetCenteredLayout();
      void SetDirectionalLayout(Direction direction);
      void SetCenteredIconTrayLayout();
      void SetDirectionalIconTrayLayout(Direction direction);

      DisplayID Display;
      DisplayInfo Info;
      bool InitialDisplaySettingsReceived;
      TaskBarAssetProxy AssetProxy;
      TaskBar::ViewData CurrentData;

      QStackedLayout* MainLayout;

      QWidget* IconTrayWidget;
      QBoxLayout* IconTrayLayout;
      QBoxLayout* SystemIconsLayout;
      QPushButton* StartButton;
      // Buttons for desktop view, search, etc. Add to SystemIconsLayout
      QGridLayout* AppIconsLayout;
      QGridLayout* AppIconsSecondaryLayout;

      QWidget* InfoWidget;
      QBoxLayout* MiscIconsWrapperLayout;
      QGridLayout* MiscIconsLayout;
      QVBoxLayout* DateTimeLayout;
      QLabel* TimeLabel;
      QLabel* DateLabel;

      QTimer AutoHideTimer;
      bool IsCenteredLayout;
      bool IsCenteredIconTrayLayout;
   };
}
