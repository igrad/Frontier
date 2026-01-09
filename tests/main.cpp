#include <gtest/gtest.h>
#include <QApplication>
#include <QtSql/QSqlQuery>
#include <QDir>
#include <QFile>

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   ::testing::InitGoogleTest(&argc, argv);

   return RUN_ALL_TESTS();
}
