#ifndef TASK_H
#define TASK_H

#include <QObject>

class Task
{
public:
    Task(QString filename);
    QString getFilename();
    bool    insertTask();
    bool    updateTask();
    static Task* getTask(QString filename);

private:
    QString filename;
};

#endif // TASK_H
