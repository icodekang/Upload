#ifndef UPLOADRECORD_H
#define UPLOADRECORD_H

#include <QObject>
#include <QList>

class UploadRecord
{
public:
    UploadRecord(QString filename, QString message, QString time, int deleted);
    QString getFilename();
    QString getMessage();
    QString getTime();
    int     getStatus();
    void    setStatus(int status);
    bool    insertRecord();
    bool    updateRecord();
    bool    deleteRecord();
    static UploadRecord* getRecord(QString filename);
    static QList<UploadRecord*>* getAllRecord();

private:
    QString filename;
    QString message;
    QString time;
    int     deleted;
};

#endif // UPLOADRECORD_H
