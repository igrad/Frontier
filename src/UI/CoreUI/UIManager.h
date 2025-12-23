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
   explicit UIManager(std::unique_ptr<BackendThreadManager>& backend);
   ~UIManager();

   void Start();

private:
   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();

   std::unique_ptr<BackendThreadManager>& Backend;
   std::unique_ptr<ShellWindow> TheShellWindow;
   std::unique_ptr<Wallpaper::WallpaperView> TheWallpaperView;
};
