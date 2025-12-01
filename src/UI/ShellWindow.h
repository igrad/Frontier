#pragma once

#include <QMainWindow>

class ShellWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit ShellWindow(QWidget *parent = nullptr);
   ~ShellWindow() = default;
};
