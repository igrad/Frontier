#pragma once

#include <QObject>

class BackendThreadManager;
class ShellWindow;

namespace Wallpaper
{
   class WallpaperView;
}

class UIManager: public QObject
{
   Q_OBJECT

public:
   explicit UIManager(BackendThreadManager* backend);
   ~UIManager();

signals:
   void UIConnectedToServiceComponents();

private slots:
   void HandleServiceThreadStarted();

private:
   void Start();

   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();

   BackendThreadManager* Backend;
   ShellWindow* TheShellWindow;
   Wallpaper::WallpaperView* TheWallpaperView;
};
