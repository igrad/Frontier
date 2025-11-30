#pragma once

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <filesystem>
#include <string>

class Logger : public QObject
{
   Q_OBJECT

public:
   explicit Logger(QObject* parent = nullptr);
   ~Logger();

public slots:
   void HandleWriteDebug(const QString& scope, const QString& string);
   void HandleWriteLog(const QString& priority,
                       const QString& scope,
                       const QString& string);

private:
   void ConnectToLog();

   void WriteToHistoryFile(const QString& log);
   void WriteToHistoryFile(const std::string& log);
   void WriteToHistoryFile(const char* log);
   void WriteToLogFile(const QString& level, const QString& scope,
      const QString& log);
   void WriteToLogFile(const std::string& level, const std::string& scope,
      const std::string& log);
   void WriteToLogFile(const char* level, const char* scope, const char* log);
   void WriteToLogFile(const std::string& str);

   std::string GetTimeString() const;

   std::string LogStr(const std::string time, const char* level,
      const char* scope, const char* log);

   std::filesystem::path LogFilePath;
   std::filesystem::path HistoryFilePath;
   QFile LogFile;
   QFile HistoryFile;
};
