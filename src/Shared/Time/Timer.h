#pragma once

#include <QTimer>

class Timer: public QObject
{
   Q_OBJECT

public:
   explicit Timer(QObject* parent = nullptr);
   Timer(int interval, bool singleShot = false, QObject* parent = nullptr);
   ~Timer() = default;

   static void AdvanceTime(int msec);
   static void UseQTimer();

   void SetInterval(int interval);
   void SetSingleShot(bool singleShot);

public slots:
   void Start();
   void Start(int interval);
   void Stop();

signals:
   void timeout();

private:
   static QSet<Timer*> AllTimers;
   static bool UsingQTimer;
   static std::chrono::time_point<std::chrono::steady_clock> Now;
   static bool InitTimeSet;

   void SetUpQTimer();

   std::chrono::time_point<std::chrono::steady_clock> StartTime;
   int Interval;
   bool SingleShot;
   bool Started;

   std::unique_ptr<QTimer> T;
};
