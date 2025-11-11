#include "../Log.h"

Log* Log::TheLogger = nullptr;

Log* Log::GetTheLogger()
{
   if(TheLogger == nullptr)
   {
      TheLogger = new Log();
   }

   return TheLogger;
}

void Log::Debug(const QString& scope, const QString& string)
{
   emit TheLogger->WriteDebug(scope, string);
}

void Log::Info(const QString& scope, const QString& string)
{
   emit TheLogger->WriteLog("INF", scope, string);
}

void Log::Warn(const QString& scope, const QString& string)
{
   emit TheLogger->WriteLog("WRN", scope, string);
}

void Log::Error(const QString& scope, const QString& string)
{
   emit TheLogger->WriteLog("ERR", scope, string);
}

void Log::Destroy()
{
   TheLogger->deleteLater();
   TheLogger = nullptr;
}

Log::Log()
{
}

Log::~Log()
{
}
