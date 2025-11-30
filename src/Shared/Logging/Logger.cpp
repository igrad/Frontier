#include "Logger.h"

#include "../Log.h"

#include <QDir>
#include <cassert>
#include <iostream>

namespace
{
   constexpr const char* LOG_FILE_NAME = "error_log.txt";
   constexpr const char* HISTORY_FILE_NAME = "history_log.txt";
}

Logger::Logger(QObject* parent)
{
   setParent(parent);

   ConnectToLog();

   // Find appdata folder
   QStringList locations =
      QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation);

   // TODO: What do we do if a writeable location cannot be found?
   // Docs say that the first result from standardLocations will be the
   // writableLocation
   assert(0 < locations.count());
   QString appDataDir =
      QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
   appDataDir.append(QDir::separator());
   appDataDir.append("Frontier");

   assert(!appDataDir.trimmed().isEmpty() &&
          "Could not find AppLocalDataLocation to write to!");

   LogFilePath = appDataDir.toStdString() + "\\" + LOG_FILE_NAME;
   HistoryFilePath = appDataDir.toStdString() + "\\" + HISTORY_FILE_NAME;

   LogFile.setFileName(LogFilePath);
   HistoryFile.setFileName(HistoryFilePath);

   // Create the folder if it does not exist
   std::cout << "Checking if " << appDataDir.toStdString() << " exists"
             << std::endl;
   if(!QDir(appDataDir).exists())
   {
      std::cout << "Creating dir" << std::endl;
      QDir().mkdir(appDataDir);
   }

   // Open files. Create them if they don't exist.
   if(!LogFile.exists())
   {
      // TODO: If this requests permissions at launch on Android devices,
      // we may need to set permissions and include WRITE_EXTERNAL_STORAGE
      // permission
      // https://doc.qt.io/qt-6/qstandardpaths.html#StandardLocation-enum
      assert(LogFile.open(QIODevice::ReadWrite) &&
             std::string("Failed to create log_file.txt at ")
                .append(LogFilePath.string())
                .c_str());
   }

   WriteToLogFile("INF", __PRETTY_FUNCTION__,
      "\n===== Logger initialized! =====");
   LogFile.close();

   if(!HistoryFile.exists())
   {
      // TODO: If this requests permissions at launch on Android devices,
      // we may need to set permissions and include WRITE_EXTERNAL_STORAGE
      // permission
      // https://doc.qt.io/qt-6/qstandardpaths.html#StandardLocation-enum
      assert(HistoryFile.open(QIODevice::ReadWrite) &&
             std::string("Failed to create history_file.txt at ")
                .append(HistoryFilePath.string())
                .c_str());
   }

   HistoryFile.close();
}

Logger::~Logger()
{
   Log::Destroy();

   LogFile.close();
   HistoryFile.close();
}

void Logger::ConnectToLog()
{
   Log* log = Log::GetTheLogger();

   connect(log, &Log::WriteLog,
           this, &Logger::HandleWriteLog);
   #ifdef QT_DEBUG
   connect(log, &Log::WriteDebug,
           this, &Logger::HandleWriteDebug);
   #endif
}

void Logger::HandleWriteDebug(const QString& scope, const QString& string)
{
   HandleWriteLog("DBG", scope, string);
}

void Logger::HandleWriteLog(const QString& priority,
                            const QString& scope,
                            const QString& string)
{
   const std::string str = LogStr(GetTimeString(),
          priority.toStdString().c_str(),
          scope.toStdString().c_str(),
          string.toStdString().c_str());
   WriteToLogFile(str);
}

void Logger::WriteToHistoryFile(const QString& log)
{
   WriteToHistoryFile(log.toStdString().c_str());
}

void Logger::WriteToHistoryFile(const std::string& log)
{
   WriteToHistoryFile(log.c_str());
}

void Logger::WriteToHistoryFile(const char* log)
{
   HistoryFile.open(QIODevice::Append);
   if(HistoryFile.isOpen())
   {
      HistoryFile.write(log);
      HistoryFile.close();
   }
   else
   {
      WriteToLogFile("ERR", __PRETTY_FUNCTION__,
         std::string("Failed to open History file. Error: ")
            .append(HistoryFile.errorString().toStdString()));
   }
}

void Logger::WriteToLogFile(const QString& level, const QString& scope,
   const QString& log)
{
   WriteToLogFile(level.toStdString().c_str(), scope.toStdString().c_str(),
      log.toStdString().c_str());
}

void Logger::WriteToLogFile(const std::string& level, const std::string& scope,
   const std::string& log)
{
   WriteToLogFile(level.c_str(), scope.c_str(), log.c_str());
}

void Logger::WriteToLogFile(const char* level, const char* scope,
   const char* log)
{
// Write to log file and to std::out if we're in debugging mode

   WriteToLogFile(LogStr(GetTimeString(), level, scope, log));
}

void Logger::WriteToLogFile(const std::string& str)
{
   LogFile.open(QIODevice::Append);
   if(LogFile.isOpen())
   {
      LogFile.write(str.c_str());
      LogFile.close();
      #ifdef QT_DEBUG
      std::cout << str.c_str() << std::flush;
      #endif
   }
   else
   {
      std::cout << "Failed to open log file. Error code: "
                << LogFile.errorString().toStdString() << std::endl;
   }
}

std::string Logger::GetTimeString() const
{
   QString monotonicTimeString =
      QString::number(QDateTime::currentMSecsSinceEpoch());
   QString dateString =
      QDateTime::currentDateTime().toString(Qt::ISODateWithMs);

   return monotonicTimeString.append(", ").append(dateString).toStdString();
}

std::string Logger::LogStr(const std::string time, const char* level,
   const char* scope, const char* log)
{
   return std::string(time)
      .append(", ")
      .append(level)
      .append(", ")
      .append(scope)
      .append(", ")
      .append(log)
      .append("\n");
}
