#include "user.h"
#include "tool.h"

User::User(QString name, QString pwd, int userId, int areaId, QString area, int status)
{
    this->name   = name;
    this->pwd    = pwd;
    this->userId = userId;
    this->areaId = areaId;
    this->area   = area;
    this->status = status;
}

QString User::getName()
{
    return name;
}

QString User::getPwd()
{
    return pwd;
}

void User::setPwd(QString pwd)
{
    this->pwd = pwd;
}

int User::getUserId()
{
    return userId;
}

int User::getAreaId()
{
    return areaId;
}

QString User::getArea()
{
    return area;
}

int User::getStatus()
{
    return status;
}

User* User::getUser()
{
    User *user = nullptr;
    QSqlQuery query(SqliteData::getDb());
    if(query.exec("select * from user where id = 0"))
    {
        if(query.first())
        {
            user = new User("", "", 0, 0, "", 0);
            user->name   = query.value(1).toString();
            user->pwd    = query.value(2).toString();
            user->userId = query.value(3).toInt();
            user->areaId = query.value(4).toInt();
            user->area   = query.value(5).toString();
            user->status = query.value(6).toInt();
        }
    }
    return user;
}

bool User::existUser()
{
    QSqlQuery query(SqliteData::getDb());
    if(query.exec("select count(*) from user"))
    {
        if(query.first())
        {
            return query.value(0).toInt();
        }
    }
    return 0;
}

bool User::insertUser()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("insert into user values(?,?,?,?,?,?,?,?,?)");
    query.bindValue(0, 0);
    query.bindValue(1, name);
    query.bindValue(2, pwd);
    query.bindValue(3, userId);
    query.bindValue(4, areaId);
    query.bindValue(5, area);
    query.bindValue(6, status);
    query.bindValue(7, Tool::getCurTime());
    query.bindValue(8, Tool::getCurTime());
    return query.exec();
}

bool User::updateUser()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("update user set name=?, password=?, userId=?, areaId=?, area=?, status=?, updateTime=? where id=?");
    query.bindValue(0, name);
    query.bindValue(1, pwd);
    query.bindValue(2, userId);
    query.bindValue(3, areaId);
    query.bindValue(4, area);
    query.bindValue(5, status);
    query.bindValue(6, Tool::getCurTime());
    query.bindValue(7, 0);
    return query.exec();
}
