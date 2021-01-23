#include "sqlitedata.h"
#include "config.h"
#include "qlog.h"
#include "tool.h"

QSqlDatabase SqliteData::db = QSqlDatabase::addDatabase("QSQLITE");

SqliteData::SqliteData()
{

}

QSqlDatabase SqliteData::getDb()
{
    return db;
}

bool SqliteData::dbInit()
{
    db.setHostName(Config::getDbHostName());
    db.setDatabaseName(Config::getDbName());
    db.setUserName(Config::getDbUserName());
    db.setPassword(Config::getDbPwdName());
    db.open();

    QSqlQuery query(SqliteData::getDb());
    bool      tableExist = false;
    if(query.exec("select count(*) from sqlite_master where type='table' and name='user'"))
    {
        if(query.first())
        {
            tableExist = query.value(0).toInt();
        }
    }
    if(!tableExist)
    {
        qInfo() << QObject::tr("user not exist");
        bool success = query.exec("create table user(\
                                   id         int primary key,\
                                   name       varchar,\
                                   password   varchar,\
                                   userId     int,\
                                   areaId     int,\
                                   area       varchar,\
                                   status     tinyint,\
                                   createTime datetime,\
                                   updateTime datetime)");
        if(success)
        {
            qDebug() << QObject::tr("user create success");
        }
        else
        {
            qDebug() << QObject::tr("user create fail");
            return -1;
        }
    }
    else
    {
        qInfo() << QObject::tr("user exist");
    }

    tableExist = false;
    if(query.exec("select count(*) from sqlite_master where type='table' and name='seting'"))
    {
        if(query.first())
        {
            tableExist = query.value(0).toInt();
        }
    }
    if(!tableExist)
    {
        qDebug() << QObject::tr("seting not exist");
        bool success = query.exec("create table seting(\
                                   id         int primary key,\
                                   times      int,\
                                   path       varchar,\
                                   createTime datetime,\
                                   updateTime datetime)");
        if(success)
        {
            qDebug() << QObject::tr("seting create success");
        }
        else
        {
            qDebug() << QObject::tr("seting create fail");
            return -1;
        }

        query.clear();
        query.prepare("insert into seting values(?,?,?,?,?)");
        query.bindValue(0, 0);
        query.bindValue(1, 5);
#ifdef Q_OS_LINUX
        query.bindValue(2, "/tmp/uploadCloud");
#else
        query.bindValue(2, "C:/uploadCloud");
#endif
        query.bindValue(3, Tool::getCurTime());
        query.bindValue(4, Tool::getCurTime());
        success = query.exec();
        if(success)
        {
            qDebug() << QObject::tr("seting init success");
        }
        else
        {
            qDebug() << QObject::tr("seting init fail");
            return -1;
        }
    }
    else
    {
        qDebug() << QObject::tr("seting exist");
    }

    tableExist = false;
    if(query.exec("select count(*) from sqlite_master where type='table' and name='task'"))
    {
        if(query.first())
        {
            tableExist = query.value(0).toInt();
        }
    }
    if(!tableExist)
    {
        qDebug() << QObject::tr("task not exist");
        bool success = query.exec("create table task(\
                                   id         integer primary key autoincrement,\
                                   filename   varchar,\
                                   createTime datetime,\
                                   updateTime datetime)");
        if(success)
        {
            qDebug() << QObject::tr("task create success");
        }
        else
        {
            qDebug() << QObject::tr("task create fail");
            return -1;
        }
    }
    else
    {
        qDebug() << QObject::tr("task exist");
    }

    tableExist = false;
    if(query.exec("select count(*) from sqlite_master where type='table' and name='alarm'"))
    {
        if(query.first())
        {
            tableExist = query.value(0).toInt();
        }
    }
    if(!tableExist)
    {
        qDebug() << QObject::tr("alarm not exist");
        bool success = query.exec("create table alarm(\
                                   id         int primary key,\
                                   success    int,\
                                   fail       int,\
                                   status     tinyint,\
                                   createTime datetime,\
                                   updateTime datetime)");
        if(success)
        {
            query.exec("insert into alarm values(0, 0, 0, 0, '2019-12-17 23:00:00', '2019-12-17 23:00:00')");
            qDebug() << QObject::tr("alarm create success");
        }
        else
        {
            qDebug() << QObject::tr("alarm create fail");
            return -1;
        }
    }
    else
    {
        qDebug() << QObject::tr("alarm exist");
    }

    tableExist = false;
    if(query.exec("select count(*) from sqlite_master where type='table' and name='record'"))
    {
        if(query.first())
        {
            tableExist = query.value(0).toInt();
        }
    }
    if(!tableExist)
    {
        qDebug() << QObject::tr("record not exist");
        bool success = query.exec("create table record(\
                                   id         integer primary key autoincrement,\
                                   filename   int,\
                                   message    int,\
                                   deleted    tinyint,\
                                   createTime datetime,\
                                   updateTime datetime)");
        if(success)
        {
            qDebug() << QObject::tr("record create success");
        }
        else
        {
            qDebug() << QObject::tr("record create fail");
            return -1;
        }
    }
    else
    {
        qDebug() << QObject::tr("record exist");
    }

    return 0;
}
