#include "seting.h"
#include "tool.h"
#include "sqlitedata.h"

Seting::Seting(int times, QString path):times(times), path(path)
{

}

int Seting::getTimes()
{
    return times;
}

QString Seting::getPath()
{
    return path;
}

Seting* Seting::getSeting()
{
    Seting *seting = nullptr;
#ifdef Q_OS_LINUX
    seting = new Seting(5, QObject::tr("/tmp/uploadCloud"));
#elif
    seting = new Seting(5, QObject::tr("C:\\uploadCloud"));
#endif
    QSqlQuery query(SqliteData::getDb());
    if(query.exec("select * from seting where id = 0"))
    {
        if(query.first())
        {
            int times = query.value(1).toInt();
            if(times > 0)
            {
                seting->times = query.value(1).toInt();
            }
            QString path = query.value(2).toString();
            if (!path.isEmpty())
            {
                seting->path = query.value(2).toString();
            }
        }
    }
    return seting;
}

bool Seting::existSeting()
{
    QSqlQuery query(SqliteData::getDb());
    if(query.exec("select count(*) from seting"))
    {
        if(query.first())
        {
            return query.value(0).toInt();
        }
    }
    return 0;
}

bool Seting::insertSeting()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("insert into seting values(?,?,?,?,?)");
    query.bindValue(0, 0);
    query.bindValue(1, times);
    query.bindValue(2, path);
    query.bindValue(3, Tool::getCurTime());
    query.bindValue(4, Tool::getCurTime());
    return query.exec();
}

bool Seting::updateSeting()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("update seting set times=?, path=?, updateTime=? where id=?");
    query.bindValue(0, times);
    query.bindValue(1, path);
    query.bindValue(2, Tool::getCurTime());
    query.bindValue(3, 0);
    return query.exec();
}
