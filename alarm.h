#ifndef ALARM_H
#define ALARM_H

#include <QObject>

class Alarm
{
public:
    Alarm(int successNum, int failNum, int status);
    int  getSuccessNum();
    int  getFailNum();
    int  getStatus();
    bool updateAlarm();
    static Alarm *getAlarm();

private:
    int successNum;
    int failNum;
    int status;
};

#endif // ALARM_H
