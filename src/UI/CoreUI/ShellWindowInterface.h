#pragma once

#include <DisplayInfo.h>

#include <QWidget>

class ShellWindowInterface: public QWidget
{
   Q_OBJECT

public:
   ~ShellWindowInterface() = default;

signals:
   void Closed();

public slots:
   virtual void HandleDisplayConfigChanged(const DisplayInfo& event) = 0;
};
