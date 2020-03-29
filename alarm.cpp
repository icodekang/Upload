#include "alarm.h"
#include "tool.h"
#include "sqlitedata.h"

Alarm::Alarm(int successNum, int failNum, int status):successNum(successNum), failNum(failNum), status(status)
{

}

int  Alarm::getSuccessNum()
{
    return successNum;
}

int  Alarm::getFailNum()
{
    return failNum;
}

int  Alarm::getStatus()
{
    return status;
}

bool Alarm::updateAlarm()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("update alarm set success=?, fail=?, status=?, updateTime=? where id=0");
    query.bindValue(0, successNum);
    query.bindValue(1, failNum);
    query.bindValue(2, status);
    query.bindValue(3, Tool::getCurTime());
    return query.exec();
}

Alarm *Alarm::getAlarm()
{
    Alarm *alarm = nullptr;
    QSqlQuery query(SqliteData::getDb());
    if(query.exec(QString("select * from alarm where id = 0")))
    {
        if(query.first())
        {
            alarm = new Alarm(query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt());
        }
    }
    return alarm;
}
