#pragma once

#include "ShellWindowInterface.h"

class ShellWindow: public ShellWindowInterface
{
   Q_OBJECT

public:
   ShellWindow(const DisplayInfo& info, QWidget* parent = nullptr);
   ~ShellWindow() = default;

public slots:
   void HandleDisplayConfigChanged(const DisplayInfo& info) override;

private slots:
   void closeEvent(QCloseEvent* event) override;

private:
   DisplayInfo Info;
};
