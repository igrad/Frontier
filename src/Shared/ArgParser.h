#pragma once

#include <QMap>
#include <QCommandLineParser>

class ArgParser: public QCommandLineParser
{
public:
   // Reminder to add to ARG_STRING_MAP in cpp
   enum class Arg: int
   {
      None = 0,
      Debug,
      TestMode,
      Dev,
      DevWindowWidth,
      DevWindowHeight,
      Enterprise
   };

   ArgParser();
   ~ArgParser() = default;

   void ParseArgs(const QCoreApplication& app);

   static QString ArgNameAsString(const Arg arg);
   static bool HasArg(const Arg arg);
   static bool RunningInCleanMode();
   static bool RunningUnitTests();
   static bool RunningWithEnterprise();
   static int GetArgAsInt(const Arg arg, const int defaultValue = 0);
   static bool GetArgAsBool(const Arg arg, const bool defaultValue = false);
   static QString GetArgAsString(const Arg arg, const QString defaultValue = "");

private:
   void SetUpArgs();
   void HandleArgParsing(const Arg arg, const bool expectsValue = false);

   static QMap<Arg, QVariant> ParsedArgs;
   static bool CleanMode;
   static bool Enterprise;

   void ArgDebug();
   void ArgTestMode();
   void ArgDev();
   void ArgDevWindowWidth();
   void ArgDevWindowHeight();
   void ArgEnterprise();
};
