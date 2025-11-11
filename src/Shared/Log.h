#pragma once

#include <QObject>

#ifndef LogDebug
#	define LogDebug(str) Log::Debug(__PRETTY_FUNCTION__, str);
#endif

#ifndef LogInfo
#	define LogInfo(str) Log::Info(__PRETTY_FUNCTION__, str);
#endif

#ifndef LogWarn
#	define LogWarn(str) Log::Warn(__PRETTY_FUNCTION__, str);
#endif

#ifndef LogError
#	define LogError(str) Log::Error(__PRETTY_FUNCTION__, str);
#endif

class Log : public QObject
{
   Q_OBJECT

public:
   static Log* GetTheLogger();

   static void Debug(const QString& scope, const QString& string);
   static void Info(const QString& scope, const QString& string);
   static void Warn(const QString& scope, const QString& string);
   static void Error(const QString& scope, const QString& string);

signals:
   void WriteDebug(const QString& scope,
                   const QString& string);
   void WriteLog(const QString& priority,
                 const QString& scope,
                 const QString& string);

protected:
   Log();
   ~Log();

   static Log* TheLogger;
   static void Destroy();

   friend class Logger;
};
