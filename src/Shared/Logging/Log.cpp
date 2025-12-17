#include "../Log.h"

Log* Log::TheLogger = nullptr;

Log* Log::GetTheLogger()
{
   if(nullptr == TheLogger)
   {
      TheLogger = new Log();
   }

   return TheLogger;
}

void Log::Debug(const QString& scope, const QString& string)
{
   if(nullptr != TheLogger)
   {
      QMetaObject::invokeMethod(TheLogger,
                                "WriteDebug",
                                Q_ARG(QString, scope),
                                Q_ARG(QString, string));
      // emit TheLogger->WriteDebug(scope, string);
   }
}

void Log::Info(const QString& scope, const QString& string)
{
   if(nullptr != TheLogger)
   {
      QMetaObject::invokeMethod(TheLogger,
                                "WriteLog",
                                Q_ARG(QString, "INF"),
                                Q_ARG(QString, scope),
                                Q_ARG(QString, string));
      // emit TheLogger->WriteLog("INF", scope, string);
   }
}

void Log::Warn(const QString& scope, const QString& string)
{
   if(nullptr != TheLogger)
   {
      QMetaObject::invokeMethod(TheLogger,
                                "WriteLog",
                                Q_ARG(QString, "WRN"),
                                Q_ARG(QString, scope),
                                Q_ARG(QString, string));
      // emit TheLogger->WriteLog("WRN", scope, string);
   }
}

void Log::Error(const QString& scope, const QString& string)
{
   if(nullptr != TheLogger)
   {
      QMetaObject::invokeMethod(TheLogger,
                                "WriteLog",
                                Q_ARG(QString, "ERR"),
                                Q_ARG(QString, scope),
                                Q_ARG(QString, string));
      // emit TheLogger->WriteLog("ERR", scope, string);
   }
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
