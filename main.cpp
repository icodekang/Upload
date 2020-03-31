#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QFile>
#include <QFont>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include "qlog.h"
#include "login.h"
#include "sqlitedata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    QFont font = a.font();
    font.setPointSize(9);
    font.setFamily("宋体");
    a.setFont(font);

    QSystemSemaphore sema("UPLOAD", 1, QSystemSemaphore::Open);
    sema.acquire();
    QSharedMemory mem("KANGKANG");
    if(!mem.create(1))
    {
        QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("程序已经运行"), QMessageBox::Yes);
        sema.release();
        return 0;
    }
    sema.release();

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QLog::initLog();
    QFile file;
    file.setFileName("log.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("请以管理员权限运行程序"), QMessageBox::Yes);
        return 0;
    }
    if(SqliteData::dbInit())
    {
        QMessageBox::critical(nullptr, QObject::tr("错误"), QObject::tr("初始化错误"), QMessageBox::Yes);
        qCritical() << "db init fail";
        return -1;
    }

    Login login;
    if(login.exec() == QDialog::Accepted)
    {
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
