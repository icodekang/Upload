#ifndef SQLITEDATA_H
#define SQLITEDATA_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>

class SqliteData
{
public:
    SqliteData();
    static bool dbInit();
    static QSqlDatabase getDb();

private:
    static QSqlDatabase db;
};

#endif // SQLITEDATA_H
