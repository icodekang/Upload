#ifndef UPLOADTHREAD_H
#define UPLOADTHREAD_H

#include <QObject>
#include <QThread>
#include <QDir>
#include <QFile>
#include <QList>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextCodec>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

class UploadThread : public QThread
{
public:
    explicit UploadThread(QObject *parent = nullptr);

public:
    void run() override;
};

#endif // UPLOADTHREAD_H
