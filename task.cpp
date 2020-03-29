#include "task.h"
#include "tool.h"
#include "sqlitedata.h"

Task::Task(QString filename):filename(filename)
{

}
QString Task::getFilename()
{
    return filename;
}

bool    Task::insertTask()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("insert into task (id, filename, createTime, updateTime) values(NULL,?,?,?)");
    query.bindValue(0, filename);
    query.bindValue(1, Tool::getCurTime());
    query.bindValue(2, Tool::getCurTime());
    return query.exec();
}

bool    Task::updateTask()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("update task set updateTime=? where filename=?");
    query.bindValue(0, Tool::getCurTime());
    query.bindValue(1, filename);
    return query.exec();
}

Task* Task::getTask(QString filename)
{
    Task *task = nullptr;
    QSqlQuery query(SqliteData::getDb());
    if(query.exec(QString("select * from task where filename = '%1'").arg(filename)))
    {
        if(query.first())
        {
            task = new Task("");
            task->filename = query.value(1).toString();
        }
    }
    return task;
}
