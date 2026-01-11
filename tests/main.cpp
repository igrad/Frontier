#include <Logging/Logger.h>

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QtSql/QSqlQuery>

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
   Logger logger;

   QApplication app(argc, argv);
   ::testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
