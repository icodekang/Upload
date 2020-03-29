#include "uploadrecord.h"
#include "sqlitedata.h"
#include "tool.h"

UploadRecord::UploadRecord(QString filename, QString message, QString time, int deleted)
{
    this->filename = filename;
    this->message  = message;
    this->time     = time;
    this->deleted   = deleted;
}

QString UploadRecord::getFilename()
{
    return filename;
}

QString UploadRecord::getMessage()
{
    return message;
}

QString UploadRecord::getTime()
{
    return time;
}

int     UploadRecord::getStatus()
{
    return deleted;
}

void    UploadRecord::setStatus(int status)
{
    this->deleted = status;
}

bool    UploadRecord::insertRecord()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("insert into record values(NULL,?,?,?,?,?)");
    query.bindValue(0, filename);
    query.bindValue(1, message);
    query.bindValue(2, deleted);
    query.bindValue(3, Tool::getCurTime());
    query.bindValue(4, Tool::getCurTime());
    return query.exec();
}

bool    UploadRecord::updateRecord()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("update record set message=?, deleted=?, updateTime=? where filename=?");
    query.bindValue(0, message);
    query.bindValue(1, deleted);
    query.bindValue(2, Tool::getCurTime());
    query.bindValue(3, filename);
    return query.exec();
}

UploadRecord* UploadRecord::getRecord(QString filename)
{
    UploadRecord *record = nullptr;
    QSqlQuery query(SqliteData::getDb());
    if(query.exec(QString("select * from record where filename = '%1'").arg(filename)))
    {
        if(query.first())
        {
            record = new UploadRecord("", "", "", 0);
            record->filename = query.value(1).toString();
            record->message  = query.value(2).toString();
            record->deleted  = query.value(3).toInt();
            record->time     = query.value(5).toString();
        }
    }
    return record;
}

QList<UploadRecord*>* UploadRecord::getAllRecord()
{
    QList<UploadRecord*> *recordList = nullptr;
    QSqlQuery query(SqliteData::getDb());
    if(query.exec("select * from record where deleted = 0 order by updateTime desc"))
    {
        recordList = new QList<UploadRecord*>();
        while(query.next())
        {
            UploadRecord *record = new UploadRecord("", "", "", 0);
            record->filename = query.value(1).toString();
            record->message  = query.value(2).toString();
            record->deleted  = query.value(3).toInt();
            record->time     = query.value(5).toString();
            recordList->append(record);
        }
    }
    return recordList;
}

bool    UploadRecord::deleteRecord()
{
    QSqlQuery query(SqliteData::getDb());
    query.prepare("delete from record where filename = ?");
    query.bindValue(0, filename);
    return query.exec();
}
