#pragma once

#include <Wallpaper/WallpaperView.h>

#include <QWidget>

class ShellWindow : public QWidget
{
   Q_OBJECT

public:
   explicit ShellWindow(QWidget* parent = nullptr);
   ~ShellWindow() = default;
};
