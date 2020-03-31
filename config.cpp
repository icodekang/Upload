#include "config.h"

//QString Config::loginUrl     = "http://wq.cloudgj.com.cn/api/index/login";
//QString Config::modifyPwdUrl = "http://wq.cloudgj.com.cn/api/index/uppwd";
//QString Config::uploadUrl    = "http://wq.cloudgj.com.cn/api/index/arr";
QString Config::loginUrl     = "192.168.1.100/api/index/login";
QString Config::modifyPwdUrl = "192.168.1.100/api/index/uppwd";
QString Config::uploadUrl    = "192.168.1.100/api/index/arr";

QString Config::dbHostName = "localhost";
QString Config::dbName     = "upload.db";
QString Config::dbUserName = "admin";
QString Config::dbPwdName  = "kangkang";

Config::Config()
{

}

QString Config::getLoginUrl()
{
    return loginUrl;
}

QString Config::getModifyPwdUrl()
{
    return modifyPwdUrl;
}

QString Config::getUploadUrl()
{
    return uploadUrl;
}

QString Config::getDbHostName()
{
    return dbHostName;
}

QString Config::getDbName()
{
    return dbName;
}

QString Config::getDbUserName()
{
    return dbUserName;
}

QString Config::getDbPwdName()
{
    return dbPwdName;
}
