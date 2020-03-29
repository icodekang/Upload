#include "tool.h"

Tool::Tool()
{

}

QString Tool::toMd5(QString str)
{
    QString strMD5;
    QByteArray qba;
    qba = QCryptographicHash::hash(str.toLatin1(), QCryptographicHash::Md5);
    strMD5.append(qba.toHex());
    return strMD5;
}

QString Tool::getCurTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

QString Tool::mkMutiDir(QString path){
    QDir dir(path);
    if ( dir.exists(path)){
        return path;
    }
    QString parentDir = mkMutiDir(path.mid(0,path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if ( !dirname.isEmpty() )
        parentPath.mkpath(dirname);
    return parentDir + "/" + dirname;
}

QStringList Tool::getFileNames(QString path)
{
    QDir dir(path);
    QStringList filter;
    filter << "*.html";
    QStringList files = dir.entryList(filter, QDir::Files | QDir::Readable, QDir::Name);
    return files;
}

int Tool::getStrNumIndex(QString str)
{
    for(int i = 0; i < str.length(); ++i)
    {
        if(str.at(i) >= '0' && str.at(i) <= '9') return i;
    }
    return -1;
}
