#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "user.h"
#include "seting.h"
#include "tool.h"
#include "alarm.h"
#include "record.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , systemTray(nullptr)
    , ut(nullptr)
    , timer(nullptr)
    , recordTimer(nullptr)
{
    ui->setupUi(this);
    ui->listWidget->item(0)->setSelected(true);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    User *user = User::getUser();
    if(user)
    {
        ui->acountShowEdit->setText(user->getName());
        ui->areaShowEdit->setText(user->getArea());
        delete user;
    }
    Seting *seting = Seting::getSeting();
    if(seting)
    {
        ui->timesEdit->setText(QString::number(seting->getTimes()));
        ui->pathEdit->setText(seting->getPath());
        QDir dir(seting->getPath());
        if(!dir.exists())
        {
            Tool::mkMutiDir(seting->getPath());
        }
        delete seting;
    }

    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(":new/icon/hospital.png"));
    systemTray->setToolTip(tr("体检报告上传工具"));
    QAction *minimumAct = new QAction(tr("立即检测"), this);
    connect(minimumAct, SIGNAL(triggered()), this, SLOT(on_uploadDetection()));
    QAction *maximumAct = new QAction(tr("发送短信"), this);
    connect(maximumAct, SIGNAL(triggered()), this, SLOT(showNormal()));
    QAction *restoreAct = new QAction(tr("打开设置面板"), this);
    connect(restoreAct, SIGNAL(triggered()), this, SLOT(showNormal()));
    QAction *showRecord = new QAction(tr("错误记录"), this);
    connect(showRecord, SIGNAL(triggered()), this, SLOT(on_showRecord()));
    QAction *quitAct = new QAction("退出程序", this);
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(systemTray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));

    QMenu *pContextMenu = new QMenu(this);
    pContextMenu->addAction(minimumAct);
    pContextMenu->addAction(maximumAct);
    pContextMenu->addAction(showRecord);
    pContextMenu->addAction(restoreAct);
    pContextMenu->addSeparator();
    pContextMenu->addAction(quitAct);
    systemTray->setContextMenu(pContextMenu);
    systemTray->show();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_checkUpload()));
    timer->start(1000);

    recordTimer = new QTimer(this);
    connect(recordTimer, SIGNAL(timeout()), this, SLOT(on_showRecord()));
    recordTimer->start(1000);
}

MainWindow::~MainWindow()
{
    if(systemTray)
    {
        delete systemTray;
    }
    if(ut)
    {
        ut->quit();
    }
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!event) return;
    if(systemTray->isVisible())
    {
        this->hide();
        systemTray->showMessage(tr("体检报告上传工具"), tr("该程序正在运行！"));
    }
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(AcountWidget == ui->stackedWidget->currentIndex())
    {
        QUrl            url(Config::getModifyPwdUrl());
        QNetworkRequest request(url);
        QHttpPart       textpart;

        QString oldPwd  = ui->oldPwd->text();
        QString newPwd1 = ui->newPwd1->text();
        QString newPwd2 = ui->newPwd2->text();
        if(oldPwd.isEmpty())
        {
            QMessageBox::warning(this, QObject::tr("提示"), QObject::tr("旧密码不能为空"), QMessageBox::Yes);
            return;
        }
        if(newPwd1.isEmpty())
        {
            QMessageBox::warning(this, QObject::tr("提示"), QObject::tr("新密码不能为空"), QMessageBox::Yes);
            return;
        }
        if(newPwd2.isEmpty())
        {
            QMessageBox::warning(this, QObject::tr("提示"), QObject::tr("确认密码不能为空"), QMessageBox::Yes);
            return;
        }
        if(newPwd1 != newPwd2)
        {
            QMessageBox::warning(this, QObject::tr("提示"), QObject::tr("新密码和确认密码不一致"), QMessageBox::Yes);
            return;
        }

        User *user = User::getUser();
        if(!user)
        {
            qCritical() << QObject::tr("user为空，用户被删除");
            return;
        }
        QHttpMultiPart *mulpart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        textpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"Id\""));
        textpart.setBody(QString::number(user->getUserId()).toUtf8());
        mulpart->append(textpart);

        textpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"Pwd\""));
        textpart.setBody(oldPwd.toUtf8());
        mulpart->append(textpart);

        textpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"New_Pwd\""));
        textpart.setBody(newPwd1.toUtf8());
        mulpart->append(textpart);

        textpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"Que_Pwd\""));
        textpart.setBody(newPwd2.toUtf8());
        mulpart->append(textpart);

        QEventLoop loop;
        QNetworkAccessManager manager;
        connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
        QNetworkReply *reply = manager.post(request, mulpart);
        mulpart->setParent(reply);
        loop.exec();
        delete mulpart;

        QByteArray repData = reply->readAll();
        delete reply;

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(repData, &jsonError));
        if(jsonError.error != QJsonParseError::NoError)
        {
            QMessageBox::warning(this, tr("提示"), tr("密码修改失败"), QMessageBox::Yes);
            qCritical() << "json error!";
            return;
        }

        QJsonObject rootObj = jsonDoc.object();
        if(!rootObj.contains("code"))
        {
            QMessageBox::warning(this, tr("提示"), tr("密码修改失败"), QMessageBox::Yes);
            qCritical() << "modify pwd fail";
            return;
        }
        int code = rootObj.value("code").toInt();
        QString message = rootObj.value("message").toString();
        if(code)
        {
            QMessageBox::warning(this, tr("提示"), message, QMessageBox::Yes);
            qInfo() << message;
            return;
        }
        user->setPwd(newPwd1);
        user->updateUser();
        delete user;
        QMessageBox::information(this, tr("提示"), tr("修改密码成功"), QMessageBox::Yes);
        ui->oldPwd->clear();
        ui->newPwd1->clear();
        ui->newPwd2->clear();
        qInfo() << "modify pwd success";
    }
    else if(UploadWidget == ui->stackedWidget->currentIndex())
    {
        QString timeStr = ui->timesEdit->text();
        QString pathStr = ui->pathEdit->text();
        if(timeStr.isEmpty())
        {
            QMessageBox::warning(this, tr("提示"), QObject::tr("上传间隔时间不能为空"), QMessageBox::Yes);
            return;
        }
        if(pathStr.isEmpty())
        {
            QMessageBox::warning(this, tr("提示"), QObject::tr("上传路径不能为空"), QMessageBox::Yes);
            return;
        }
        Seting seting(timeStr.toInt(), pathStr);
        Tool::mkMutiDir(pathStr);
        QDir dir(pathStr);
        if(dir.exists())
        {
            if(!seting.existSeting())
            {
                if(seting.insertSeting())
                {
                    QMessageBox::information(this, tr("提示"), QObject::tr("修改成功"), QMessageBox::Yes);
                }
                else
                {
                    qCritical() << "insert seting fail";
                    QMessageBox::warning(this, tr("提示"), QObject::tr("修改失败"), QMessageBox::Yes);
                }
            }
            else
            {
                if(seting.updateSeting())
                {
                    QMessageBox::information(this, tr("提示"), QObject::tr("修改成功"), QMessageBox::Yes);
                }
                else
                {
                    qCritical() << "update seting fail";
                    QMessageBox::warning(this, tr("提示"), QObject::tr("修改失败"), QMessageBox::Yes);
                }
            }
        }
        else
        {
            if(dir.mkdir(pathStr))
            {
                qInfo() << QString("create %1 success").arg(pathStr);
                if(!seting.existSeting())
                {
                    if(seting.insertSeting())
                    {
                        QMessageBox::information(this, tr("提示"), QObject::tr("修改成功"), QMessageBox::Yes);
                    }
                    else
                    {
                        qCritical() << "insert seting fail";
                        QMessageBox::warning(this, tr("提示"), QObject::tr("修改失败"), QMessageBox::Yes);
                    }
                }
                else
                {
                    if(seting.updateSeting())
                    {
                        QMessageBox::information(this, tr("提示"), QObject::tr("修改成功"), QMessageBox::Yes);
                    }
                    else
                    {
                        qCritical() << "update seting fail";
                        QMessageBox::warning(this, tr("提示"), QObject::tr("修改失败"), QMessageBox::Yes);
                    }
                }
            }
            else
            {
                qCritical() << QString("create %1 fail").arg(pathStr);
                QMessageBox::warning(this, tr("提示"), QObject::tr("请确认路径是否合法"), QMessageBox::Yes);
            }
        }
    }
    else if(MessageWidget == ui->stackedWidget->currentIndex())
    {
        QMessageBox::information(this, tr("提示"), QObject::tr("程序员正在加班实现中..."), QMessageBox::Yes);
    }
}

void MainWindow::on_pushButton_1_clicked()
{
    if(AcountWidget == ui->stackedWidget->currentIndex())
    {
        ui->oldPwd->clear();
        ui->newPwd1->clear();
        ui->newPwd2->clear();
        ui->oldPwd->setFocus();
    }
    else if(UploadWidget == ui->stackedWidget->currentIndex())
    {
        Seting *seting = Seting::getSeting();
        if(seting)
        {
            ui->timesEdit->setText(QString::number(seting->getTimes()));
            ui->pathEdit->setText(seting->getPath());
            delete seting;
        }
    }
    else if(MessageWidget == ui->stackedWidget->currentIndex())
    {
        ui->msgRadioButton->setChecked(false);
        ui->msgTextEdit->clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, QObject::tr("选择文件夹"), "/");
    if (path.isEmpty())
    {
        return;
    }
    else
    {
        ui->pathEdit->setText(path);
    }
}

void MainWindow::on_uploadDetection()
{
    Seting *seting = Seting::getSeting();
    if(!seting || seting->getPath().isEmpty() || !seting->getTimes())
    {
        QMessageBox::warning(this, tr("提示"), QObject::tr("请先进行上传设置"), QMessageBox::Yes);
        return;
    }
    QString dir = seting->getPath();
    delete seting;
    systemTray->showMessage(tr("消息"), tr("正在进行上传检测"));

    if(ut && ut->isRunning())
    {
        systemTray->showMessage(tr("消息"), tr("检测程序正在运行"));
        return;
    }
    qInfo() << "start upload thread";
    ut = new UploadThread();
    ut->start();
}

void MainWindow::on_checkUpload()
{
    Alarm *alarm = Alarm::getAlarm();
    if(alarm)
    {
        if(alarm->getStatus())
        {
            systemTray->showMessage(tr("体检上传结果"), QString("%1:%2 %3:%4").arg(tr("成功")).arg(alarm->getSuccessNum()).arg(tr("失败")).arg(alarm->getFailNum()));
            Alarm upAlarm(alarm->getSuccessNum(), alarm->getFailNum(), 0);
            upAlarm.updateAlarm();
        }
    }
    delete alarm;
}

void MainWindow::on_showRecord()
{
    Record *record = new Record();
    record->show();
    if(recordTimer)
    {
        recordTimer->stop();
        recordTimer = nullptr;
    }
}
