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

   void Start();

private:
   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();

   BackendThreadManager* Backend;
   ShellWindow* TheShellWindow;
   Wallpaper::WallpaperView* TheWallpaperView;
};
