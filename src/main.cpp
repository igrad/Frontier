#include "ArgParser.h"
#include <Logging/Logger.h>
#include <Log.h>
#include <ShellWindow.h>

#include <QApplication>

namespace
{
   Logger* LOGGER = nullptr;
}

// Only components on the heap should be set up here.
// Making sure a matching delete or deleteLater is in tear down.
void SetUpComponents(QApplication* app)
{
}

void TearDownComponents()
{
   // Last
   LOGGER->deleteLater();
   LOGGER = nullptr;
}

int main(int argc, char *argv[])
{
   // LOGGER is a special case since we need logging before launching the app
   LOGGER = new Logger(nullptr);
   LogInfo("Launching Frontier");

   QApplication app(argc, argv);
   ArgParser argParser;
   argParser.ParseArgs(app);

   SetUpComponents(&app);

   ShellWindow w;
   w.show();

   const int rVal = app.exec();

   TearDownComponents();

   return rVal;
}
