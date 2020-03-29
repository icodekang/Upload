#ifndef QLOG_H
#define QLOG_H

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <qapplication.h>

class QLog
{
public:
    QLog();
    static void initLog();
};

#endif // QLOG_H
