#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include <QFile>
#include <QFont>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include "mainwindow.h"
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

    QSystemSemaphore sema("KANGKANG", 1, QSystemSemaphore::Open);
    sema.acquire();

#ifdef Q_OS_LINUX
    QSharedMemory mem("UPLOAD");
    if (mem.attach())
    {
        mem.detach();
    }
#endif
    QSharedMemory unimem("UPLOAD");
    bool isRunning = false;
    if(unimem.attach())
    {
        isRunning = true;
    }
    else
    {
        unimem.create(1);
        isRunning = false;
    }
    sema.release();

    if(isRunning)
    {
        QMessageBox::information(nullptr, QObject::tr("提示"), QObject::tr("程序已经运行"), QMessageBox::Yes);
        exit(0);
    }

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
