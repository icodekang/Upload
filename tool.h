#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QDateTime>
#include <QDir>
#include <QCryptographicHash>

class Tool
{
public:
    Tool();
    static QString toMd5(QString str);
    static QString getCurTime();
    static QString mkMutiDir(QString path);
    static QStringList getFileNames(QString path);
    static int getStrNumIndex(QString str);
};

#endif // TOOL_H
