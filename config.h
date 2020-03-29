#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>

class Config
{
public:
    Config();
    static QString getLoginUrl();
    static QString getModifyPwdUrl();
    static QString getUploadUrl();

    static QString getDbHostName();
    static QString getDbName();
    static QString getDbUserName();
    static QString getDbPwdName();

private:
    static QString loginUrl;
    static QString modifyPwdUrl;
    static QString uploadUrl;

private:
    static QString dbHostName;
    static QString dbName;
    static QString dbUserName;
    static QString dbPwdName;
};

#endif // CONFIG_H
