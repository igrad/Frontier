#include "ArgParser.h"
#include <Logging/Logger.h>
#include <Log.h>
#include "Backend/SettingsService/SettingsService.h"
#include <ShellWindow.h>

#include <QApplication>

namespace
{
   Logger* LOGGER = nullptr;
   Settings::SettingsService* SETTINGS_SERVICE = nullptr;
}

// Only components on the heap should be set up here.
// Making sure a matching delete or deleteLater is in tear down.
void SetUpComponents(QApplication* app)
{
   SETTINGS_SERVICE = new Settings::SettingsService;
}

void TearDownComponents()
{
   // Last
   LOGGER->deleteLater();
   LOGGER = nullptr;

   SETTINGS_SERVICE->deleteLater();
   SETTINGS_SERVICE = nullptr;
   // First
}

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);

   ArgParser argParser;
   argParser.ParseArgs(app);

   if(ArgParser::GetArgAsBool(ArgParser::Arg::Debug, false))
   {
      #ifndef QT_DEBUG
      #define QT_DEBUG
      #endif
   }

   // LOGGER is a special case since we need logging before launching the app
   LOGGER = new Logger(nullptr);
   if(ArgParser::RunningInCleanMode())
   {
      LogInfo("Running in clean mode");
   }
   LogInfo("Launching Frontier");
   LogInfo(QString("Launching with argc: %1 and args").arg(argc));
   for(int iter = 1; iter < argc; ++iter)
   {
      LogInfo(QString("arg %1: %2").arg(iter).arg(argv[iter]));
   }

   SetUpComponents(&app);

   ShellWindow w;
   w.show();

   const int rVal = app.exec();

   TearDownComponents();

   return rVal;
}
