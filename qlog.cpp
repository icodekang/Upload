#include "qlog.h"

QLog::QLog()
{

}

void outputMessage(QtMsgType logtype, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QString typeStr;

    switch(logtype)
    {
    case QtDebugMsg:
        typeStr = QString("[Debug]   ");
        break;

    case QtInfoMsg:
        typeStr = QString("[Info]    ");
        break;

    case QtWarningMsg:
        typeStr = QString("[Warning] ");
        break;

    case QtCriticalMsg:
        typeStr = QString("[Critical]");
        break;

    case QtFatalMsg:
        typeStr = QString("[Fatal]   ");
        break;
    }

    // 设置输出信息格式
    QStringList context_list = QString(context.file).split("\\");
    QString file_name = context_list.last();
    QString context_info = QString("%1:%2").arg(file_name).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("[%1]").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(current_date).arg(typeStr).arg(context_info).arg(msg);

    // 输出信息至文件中（读写、追加形式
    QString filepath("./log.txt");
    QFile file(filepath);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

void QLog::initLog()
{
    qInstallMessageHandler(outputMessage);
}
