#pragma once

#include "ShellWindowInterface.h"

class ShellWindow: public ShellWindowInterface
{
   Q_OBJECT

public:
   explicit ShellWindow(QWidget* parent = nullptr);
   ~ShellWindow() = default;

private slots:
   void closeEvent(QCloseEvent* event) override;
};
