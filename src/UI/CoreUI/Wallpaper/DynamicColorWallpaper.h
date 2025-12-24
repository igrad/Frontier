#pragma once

#include <QColor>
#include <QPropertyAnimation>
#include <QWidget>

namespace Wallpaper
{
   class DynamicColorWallpaper: public QWidget
   {
      Q_OBJECT
      Q_PROPERTY(QColor Color READ GetColor WRITE SetColor NOTIFY ColorUpdated)

   public:
      explicit DynamicColorWallpaper(QWidget* parent = nullptr);
      ~DynamicColorWallpaper() = default;

      QColor GetColor() const;
      void SetColor(const QColor& color);
      void SetColors(const QList<QColor>& colors);
      void SetDurationPerColor(const quint64& ms);
      void Start();

   signals:
      void ColorUpdated(const QColor& color);	// Don't use this

   protected:
      void paintEvent(QPaintEvent* event) override;

   private:
      QColor Color;
      QList<QColor> Colors;
      quint64 Duration;
      std::unique_ptr<QPropertyAnimation> Animation;
   };
}
