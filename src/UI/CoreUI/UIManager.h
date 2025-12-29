#pragma once

#include <QObject>

class BackendThreadManager;
class DataAccessThreadManager;
class ShellWindow;

namespace Wallpaper
{
   class WallpaperView;
}

class UIManager: public QObject
{
   Q_OBJECT

public:
   explicit UIManager(DataAccessThreadManager* dataAccess,
                      BackendThreadManager* backend);
   ~UIManager();

signals:
   void UIConnectedToServiceComponents();
   void ShellWindowClosed();

private slots:
   void HandleServiceThreadStarted();

private:
   void Start();

   void BuildUIComponents();
   void BuildTheShellWindow();
   void BuildTheWallpaperView();

   DataAccessThreadManager* DataAccess;
   BackendThreadManager* Backend;
   ShellWindow* TheShellWindow;
   Wallpaper::WallpaperView* TheWallpaperView;
};
