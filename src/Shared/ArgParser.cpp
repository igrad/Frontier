#include "ArgParser.h"

#include <QVariant>

typedef ArgParser::Arg Arg;

QMap<Arg, QVariant> ArgParser::ParsedArgs;

ArgParser::ArgParser()
   : QCommandLineParser()
{
   setApplicationDescription("Frontier - Windows Shell GUI Replacement");

   SetUpArgs();
}

void ArgParser::SetUpArgs()
{
   ArgDev();
   ArgDevWindowWidth();
   ArgDevWindowHeight();
}

QString ArgParser::ArgNameAsString(const Arg arg)
{
   static const QMap<Arg, QString> ARG_STRING_MAP =
   {
      { Arg::None, "None" },
      { Arg::Dev, "Dev" },
      { Arg::DevWindowWidth, "DevWindowWidth" },
      { Arg::DevWindowHeight, "DevWindowHeight" },
      // { Arg::, "" },
   };

   return ARG_STRING_MAP.value(arg);
}

bool ArgParser::HasArg(const Arg arg)
{
   return ParsedArgs.contains(arg);
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
      QCoreApplication::translate("ArgParser", "Dev window width")
   });
}

void ArgParser::ArgDevWindowHeight()
{
   addOption({
      { "dh", "dev-window-height" },
      QCoreApplication::translate("ArgParser", "Dev window height")
   });
}

void ArgParser::ParseArgs(const QCoreApplication& app)
{
   process(app);
}
