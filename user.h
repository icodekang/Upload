#ifndef USER_H
#define USER_H

#include <QObject>
#include "sqlitedata.h"

class User
{
public:
    User(QString name, QString pwd, int userId, int areaId, QString area, int status);
    QString getName();
    QString getPwd();
    void    setPwd(QString pwd);
    int     getUserId();
    int     getAreaId();
    QString getArea();
    int     getStatus();

public:
    bool existUser();
    bool insertUser();
    bool updateUser();
    static User* getUser();

private:
    QString   name;
    QString   pwd;
    int       userId;
    int       areaId;
    QString   area;
    int       status;
};

#endif // USER_H
