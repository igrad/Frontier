#pragma once

#include <QtSql/QSqlQuery>

class SqlQuery: public QSqlQuery
{
public:
   // Overrides defaulted constructor since it's literally garbage
   SqlQuery(const QString& query = QString(),
            const QSqlDatabase& db = QSqlDatabase()) = delete;

   // Use this constructor and pass in the OPEN!!! QSqlDatabase
   SqlQuery(const QSqlDatabase& db)
      : QSqlQuery(db)
   {}
};
