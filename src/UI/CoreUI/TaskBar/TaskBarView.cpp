#include "TaskBarView.h"

#include <TaskBarServiceInterface.h>
#include <ShellWindowInterface.h>

#include <QGraphicsOpacityEffect>

using namespace TaskBar;

namespace
{
   constexpr QBoxLayout::Direction ToQtDirection(TaskBar::Direction dir)
   {
      switch(dir)
      {
      case Direction::RightToLeft:
         return QBoxLayout::Direction::RightToLeft;
      default:
         return QBoxLayout::Direction::LeftToRight;
      }
   }
}

TaskBarView::TaskBarView(XPtr<TaskBarServiceInterface> service,
                         ShellWindowInterface* window,
                         const DisplayInfo& info)
   : QWidget(window)
   , Display(info.ID)
   , Info(info)
   , CurrentData()
   , InitialDisplaySettingsReceived(false)
   , IsCenteredLayout(false)
   , IsCenteredIconTrayLayout(false)
{
   CreateUI();
   ConnectToServiceSignals(service);
   ConnectToAssetProxy();
}

void TaskBarView::HandleViewDataChanged(const DisplayID& displayID, const TaskBar::ViewData& data)
{
   if(!InitialDisplaySettingsReceived)
   {
      InitialDisplaySettingsReceived = true;
      HandleInitialViewData(displayID, data);

      CurrentData = data;
      return;
   }

   if(data == CurrentData)
   {
      return;
   }

   if(data.Alignment != CurrentData.Alignment)
   {

   }

   if(data.DisplayID != CurrentData.DisplayID)
   {
      // TODO: Make sure we're processing the settings update for the correct monitor!
   }

   if(data.AutoHide != CurrentData.AutoHide)
   {

   }

   if(data.AutoHideDelayMs != CurrentData.AutoHideDelayMs)
   {

   }

   if(data.Direction != CurrentData.Direction ||
       data.Alignment != CurrentData.Alignment)
   {
      SetLayout(data);
   }

   if(data.Opacity != CurrentData.Opacity)
   {
      QGraphicsOpacityEffect* effect = nullptr;
      if(data.Opacity != 100)
      {
         effect = new QGraphicsOpacityEffect(this);
         effect->setOpacity(data.Opacity / 100);
      }

      setGraphicsEffect(effect);
   }

   if(data.StartButtonShown != CurrentData.StartButtonShown)
   {
      StartButton->show();
   }

   if(data.StartButtonImagePath != CurrentData.StartButtonImagePath)
   {
      SetStartButtonImagePath(data.StartButtonImagePath);
   }

   CurrentData = data;
}

void TaskBarView::HandleInitialViewData(const DisplayID& displayID, const TaskBar::ViewData& data)
{
   // Just set each member with no regard to differences. This ensures that if the field is
   // blank, then we initialize with default settings and assets.
   SetStartButtonImagePath(data.StartButtonImagePath);
}

void TaskBarView::HandleStartButtonImageReady(const QPixmap& pix)
{
   const QIcon icon(pix);
   StartButton->setIcon(icon);
   const QList<QSize> sizes = icon.availableSizes();
   StartButton->setIconSize(sizes[0]); // WIP
}

void TaskBarView::CreateUI()
{
   MainLayout = new QStackedLayout();
   MainLayout->setStackingMode(QStackedLayout::StackingMode::StackAll);

   // Icon tray (system and app icons, but not misc icons in the info widget)
   IconTrayWidget = new QWidget();
   IconTrayLayout = new QHBoxLayout();

   // System icons
   SystemIconsLayout = new QHBoxLayout();
   IconTrayWidget->setLayout(SystemIconsLayout);
   StartButton = new QPushButton();
   // const QIcon defaultIcon();
   // StartButton->setIcon()
   SystemIconsLayout->addWidget(StartButton);
   IconTrayLayout->addLayout(SystemIconsLayout);

   // App icons
   AppIconsLayout = new QGridLayout();
   IconTrayLayout->addLayout(AppIconsLayout);
   IconTrayWidget->setLayout(IconTrayLayout);

   // Info widget
   InfoWidget = new QWidget();
   MiscIconsWrapperLayout = new QHBoxLayout();
   // This spacer moves the misc icons all the way to the right/bottom. Might change this one day.
   MiscIconsWrapperLayout->insertSpacerItem(0, new QSpacerItem(0, 0, QSizePolicy::Expanding));

   // Misc icons
   MiscIconsLayout = new QGridLayout();
   // Add misc icons. Network, sound, system update, expand button, etc.
   MiscIconsWrapperLayout->addLayout(MiscIconsLayout);

   // Date/time
   DateTimeLayout = new QVBoxLayout();
   TimeLabel = new QLabel("23:59");
   DateLabel = new QLabel("12/31/2026");
   DateTimeLayout->addWidget(TimeLabel);
   DateTimeLayout->addWidget(DateLabel);
   MiscIconsWrapperLayout->addLayout(DateTimeLayout);
   InfoWidget->setLayout(MiscIconsWrapperLayout);

   // Add widgets in z-index order from back to front
   MainLayout->addWidget(InfoWidget);
   MainLayout->addWidget(IconTrayWidget);

   setLayout(MainLayout);

   // NOTE: Just for dev to get something on screen. Should be handled initially by DisplayInfo
   // and then updated by settings
   setGeometry({0, 100, 1920, 100});
   show();
}

void TaskBarView::ConnectToServiceSignals(XPtr<TaskBarServiceInterface> service)
{
   if(service.isNull())
   {
      LogError("TaskBarService should have been created first!");
      return;
   }

   connect(service.get(), &TaskBarServiceInterface::ViewDataChanged,
           this, &TaskBarView::HandleViewDataChanged);
}

void TaskBarView::ConnectToAssetProxy()
{
   connect(&AssetProxy, &TaskBarAssetProxy::StartButtonImageReady,
           this, &TaskBarView::HandleStartButtonImageReady);
}

void TaskBarView::SetLayout(const TaskBar::ViewData& data)
{
   const Direction newDir = (Direction::None != data.Direction) ?
                               data.Direction :
                               Direction::LeftToRight;
   if(Direction::Centered == newDir)
   {
      // Changing to a centered layout
      SetCenteredLayout();
   }
   else
   {
      // Changing to a directional layout
      SetDirectionalLayout(newDir);
   }
}

void TaskBarView::SetDirection(Direction direction)
{
   switch(direction)
   {
   case Direction::LeftToRight:
   case Direction::RightToLeft:
      // MainLayout->setDirection(QVBoxLayout::RightToLeft);
      SetDirectionalLayout(direction);
      break;
   case Direction::Centered:
      SetCenteredLayout();
      break;
   default:
      break;
   }
}

// Add SetDirection for icon direction, too

void TaskBarView::SetStartButtonImagePath(const QString& path)
{
   AssetProxy.LoadStartButtonImage(path.isEmpty() ?
                                      ToAssetPath(Assets::ImageName::StartMenuImage) :
                                      path);
}

void TaskBarView::SetCenteredLayout()
{
   if(!IsCenteredLayout)
   {
      IconTrayLayout->setAlignment(Qt::AlignCenter);
   }

   IsCenteredLayout = true;
}

void TaskBarView::SetDirectionalLayout(Direction direction)
{
   if(IsCenteredLayout)
   {
      IconTrayLayout->setAlignment(Qt::AlignLeft);
   }

   IsCenteredLayout = false;
}

void TaskBarView::SetCenteredIconTrayLayout()
{
   if(!IsCenteredIconTrayLayout)
   {
      IconTrayLayout->addLayout(AppIconsSecondaryLayout);
   }

   IsCenteredIconTrayLayout = true;
}

void TaskBarView::SetDirectionalIconTrayLayout(Direction direction)
{
   if(IsCenteredIconTrayLayout)
   {
      // Centered Direction for the icon tray layout uses two layouts, so now we have to merge
      // the two layouts back down to 1 for directional display.
      QList<QWidget*> children = AppIconsSecondaryLayout->findChildren<QWidget*>();
      for(QWidget* child : std::as_const(children))
      {
         AppIconsLayout->addWidget(child);
      }

      IconTrayLayout->removeItem(AppIconsSecondaryLayout);
   }

   IconTrayLayout->setDirection(ToQtDirection(direction));
   IsCenteredIconTrayLayout = false;
}
