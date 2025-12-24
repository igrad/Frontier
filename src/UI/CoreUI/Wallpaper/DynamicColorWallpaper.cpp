#include "DynamicColorWallpaper.h"

#include <QPainter>
#include <QPropertyAnimation>

using namespace Wallpaper;

DynamicColorWallpaper::DynamicColorWallpaper(QWidget* parent)
   : QWidget(parent)
   , Colors()
   , Duration(1000)
   , Animation(nullptr)
{
}

QColor DynamicColorWallpaper::GetColor() const
{
   return Color;
}

void DynamicColorWallpaper::SetColor(const QColor& color)
{
   if(color != Color)
   {
      Color = color;
      update();
   }
}

void DynamicColorWallpaper::SetColors(const QList<QColor>& colors)
{
   Colors = colors;
}

void DynamicColorWallpaper::SetDurationPerColor(const quint64& ms)
{
   Duration = ms;
}

void DynamicColorWallpaper::Start()
{
   Animation.reset(new QPropertyAnimation(this, "Color"));
   Animation->setDuration(Duration * Colors.count());

   for(int iter = 0; iter < Colors.count(); ++iter)
   {
      const double step = iter/(Colors.count() + 1);
      Animation->setKeyValueAt(step, Colors[iter]);
   }
   // Ensure we loop back to our starting color nicely
   Animation->setKeyValueAt(1.0, Colors[0]);

   Animation->setLoopCount(-1);
   Animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void DynamicColorWallpaper::paintEvent(QPaintEvent* event)
{
   QPainter painter(this);
   painter.fillRect(rect(), Color);
}
