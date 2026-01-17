#pragma once

#include "DisplayInfo.h"

#include <QObject>

class DisplaysManagerInterface: public QObject
{
   Q_OBJECT

public:
   ~DisplaysManagerInterface();

signals:
   void DisplayConfigChanged(const DisplayEvent& event,
                             const QSet<DisplayInfo>& displays);
};
