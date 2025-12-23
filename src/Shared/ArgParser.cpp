#include "ArgParser.h"

#include <QVariant>

namespace
{
   const char* UNIT_TEST_EXECUTABLE_NAME = "FrontierUnitTests";
}

typedef ArgParser::Arg Arg;

QMap<Arg, QVariant> ArgParser::ParsedArgs;
bool ArgParser::CleanMode = false;

ArgParser::ArgParser()
   : QCommandLineParser()
{
   setApplicationDescription("Frontier - Windows Shell GUI Replacement");

   SetUpArgs();
}

void ArgParser::SetUpArgs()
{
   ArgDebug();
   ArgDev();
   ArgTestMode();
   ArgDevWindowWidth();
   ArgDevWindowHeight();
}

QString ArgParser::ArgNameAsString(const Arg arg)
{
   static const QMap<Arg, QString> ARG_STRING_MAP =
   {
      { Arg::None, "none" },
      { Arg::Debug, "debug" },
      { Arg::TestMode, "test-mode" },
      { Arg::Dev, "dev" },
      { Arg::DevWindowWidth, "dev-window-width" },
      { Arg::DevWindowHeight, "dev-window-height" },
      // { Arg::, "" },
   };

   return ARG_STRING_MAP.value(arg);
}

bool ArgParser::HasArg(const Arg arg)
{
   return ParsedArgs.contains(arg);
}

bool ArgParser::RunningInCleanMode()
{
   return CleanMode;
}

bool ArgParser::RunningUnitTests()
{
   return (UNIT_TEST_EXECUTABLE_NAME == QCoreApplication::applicationName());
}

int ArgParser::GetArgAsInt(const Arg arg, const int defaultValue)
{
   return ParsedArgs.value(arg, defaultValue).toInt();
}

bool ArgParser::GetArgAsBool(const Arg arg, const bool defaultValue)
{
   return ParsedArgs.value(arg, defaultValue).toBool();
}

QString ArgParser::GetArgAsString(const Arg arg, const QString defaultValue)
{
   return ParsedArgs.value(arg, defaultValue).toString();
}

void ArgParser::ArgDebug()
{
   addOption({
      { "D", "debug" },
      QCoreApplication::translate("ArgParser", "Launch Frontier in debug mode")
   });
}

void ArgParser::ArgTestMode()
{
   addOption({
      "test-mode",
      QCoreApplication::translate("ArgParser", "Launch Frontier in test mode")
   });
}

void ArgParser::ArgDev()
{
   addOption({
      { "d", "dev" },
      QCoreApplication::translate("ArgParser", "Launch Frontier in windowed dev mode")
   });
}

void ArgParser::ArgDevWindowWidth()
{
   addOption({
      { "dw", "dev-window-width" },
      QCoreApplication::translate("ArgParser", "Dev window width"),
      "dev-window-width"
   });
}

void ArgParser::ArgDevWindowHeight()
{
   addOption({
      { "dh", "dev-window-height" },
      QCoreApplication::translate("ArgParser", "Dev window height"),
      "dev-window-height"
   });
}

void ArgParser::HandleArgParsing(const Arg arg, const bool expectsValue)
{
   const QString str = ArgNameAsString(arg);
   if(isSet(str))
   {
      QVariant val = expectsValue ? value(str) : QVariant(true);
      ArgParser::ParsedArgs[arg] = val;
   }
}

void ArgParser::ParseArgs(const QCoreApplication& app)
{
   process(app);

   // This could definitely be done in a smarter, more programmatic way
   // TODO: Improve this crap
   HandleArgParsing(Arg::Debug);
   HandleArgParsing(Arg::TestMode);
   HandleArgParsing(Arg::Dev);
   HandleArgParsing(Arg::DevWindowWidth);
   HandleArgParsing(Arg::DevWindowHeight);

   if(ArgParser::GetArgAsBool(Arg::Dev, false) ||
       ArgParser::GetArgAsBool(Arg::TestMode, false))
   {
      ArgParser::CleanMode = true;
   }
}
