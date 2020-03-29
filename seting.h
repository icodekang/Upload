#ifndef SETING_H
#define SETING_H

#include <QObject>

class Seting
{
public:
    Seting(int times, QString path);
    int     getTimes();
    QString getPath();
    bool    existSeting();
    bool    insertSeting();
    bool    updateSeting();
    static Seting* getSeting();

private:
    int     times;
    QString path;
};

#endif // SETING_H
