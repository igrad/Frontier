#include "Timer.h"

#include <Log.h>

#include <QSet>

namespace
{
   constexpr int DEFAULT_INTERVAL = 1000;
   constexpr bool DEFAULT_SINGLESHOT = false;
}

QSet<Timer*> Timer::AllTimers;
bool Timer::UsingQTimer = false;

void Timer::AdvanceTime(int msec)
{
   if(UsingQTimer)
   {
      LogError("Timer::AdvanceTimer must only be used in testing!");
      return;
   }
   const auto now = std::chrono::steady_clock::now();

   for(Timer* timer : std::as_const(AllTimers))
   {
      if(timer->Started &&
          (timer->StartTime + std::chrono::milliseconds(timer->Interval) < now))
      {
         emit timer->timeout();

         if(!timer->SingleShot)
         {
            timer->Start();
         }
      }
   }
}

void Timer::UseQTimer()
{
   UsingQTimer = true;
}

Timer::Timer(QObject* parent)
   : QObject(parent)
   , Interval(DEFAULT_INTERVAL)
   , SingleShot(DEFAULT_SINGLESHOT)
   , Started(false)
{
   SetUpQTimer();
   AllTimers.insert(this);
}

Timer::Timer(int interval, bool singleShot, QObject* parent)
   : QObject(parent)
   , Interval(interval)
   , SingleShot(singleShot)
   , Started(false)
{
   SetUpQTimer();
   AllTimers.insert(this);
}

void Timer::SetInterval(int interval)
{
   Interval = interval;

   if(UsingQTimer)
   {
      T->setInterval(interval);
   }
}

void Timer::SetSingleShot(bool singleShot)
{
   SingleShot = singleShot;

   if(UsingQTimer)
   {
      T->setSingleShot(singleShot);
   }
}

void Timer::Start()
{
   StartTime = std::chrono::steady_clock::now();
   Started = true;

   if(UsingQTimer)
   {
      T->start();
   }
}

void Timer::Start(int interval)
{
   Interval = interval;

   Start();
}

void Timer::Stop()
{
   Started = false;

   if(UsingQTimer)
   {
      T->stop();
   }
}

void Timer::SetUpQTimer()
{
   if(UsingQTimer)
   {
      T.reset(new QTimer(this));
      T->setInterval(Interval);
      T->setSingleShot(SingleShot);

      connect(T.get(), &QTimer::timeout,
              this, &Timer::timeout,
              Qt::UniqueConnection);
   }
}
