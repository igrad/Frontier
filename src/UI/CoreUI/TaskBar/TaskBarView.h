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
                  uint8_t displayNumber);
      ~TaskBarView() = default;

   public:
      void HandleViewDataChanged(uint8_t display, const TaskBar::ViewData& data);

   private slots:
      void HandleImageReady(Assets::ImageName name, const QPixmap& image);

   private:
      void CreateUI();
      void ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service);
      void ConnectToAssetClientSignals();

      uint8_t DisplayNum;
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
