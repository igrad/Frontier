#pragma once

#include <QWidget>

class ShellWindowInterface: public QWidget
{
   Q_OBJECT

public:
   ~ShellWindowInterface() = default;

signals:
   void Closed();
};
