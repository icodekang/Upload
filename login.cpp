#include <QTextCodec>
#include "login.h"
#include "config.h"
#include "user.h"
#include "ui_login.h"

Login::Login(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    User *user = User::getUser();
    if(user && user->getStatus())
    {
        ui->userEdit->setText(user->getName());
        ui->pwdEdit->setText(user->getPwd());
        ui->pwdCheckBox->setCheckState(Qt::Checked);
    }
    delete user;
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_2_clicked()
{
    QString user = QTextCodec::codecForName("GBK")->fromUnicode(this->ui->userEdit->text());
    if(!user.isEmpty())
    {
        qInfo("user: %s login close", qPrintable(user));
    }
    this->close();
}

void Login::on_loginBtn_clicked()
{
    QUrl            url(Config::getLoginUrl());
    QNetworkRequest request(url);
    QHttpPart       textpart;

    if(ui->userEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("请输入用户名"), QMessageBox::Yes);
        return;
    }

    if(ui->pwdEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("提示"), tr("请输入密码"), QMessageBox::Yes);
        return;
    }

    QHttpMultiPart *mulpart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    textpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"UserName\""));
    textpart.setBody(ui->userEdit->text().toUtf8());
    mulpart->append(textpart);

    textpart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"UserPwd\""));
    textpart.setBody(ui->pwdEdit->text().toUtf8());
    mulpart->append(textpart);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->post(request, mulpart);
    connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply* reply){
        mulpart->setParent(reply);
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if(statusCode.isValid())
        {
            if(statusCode.toInt() != 200)
            {
                QMessageBox::warning(this, tr("提示"), tr("登录失败"), QMessageBox::Yes);
                return;
            }
        }

        QNetworkReply::NetworkError error = reply->error();
        if(error != QNetworkReply::NoError) {
            qDebug() << "login failed: " << reply->errorString();
            QMessageBox::warning(this, tr("提示"), tr("登录失败"), QMessageBox::Yes);
            return;
        }

        QByteArray repData = reply->readAll();
        delete reply;

        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(repData, &jsonError));
        if(jsonError.error != QJsonParseError::NoError)
        {
            QMessageBox::warning(this, tr("提示"), tr("登录失败"), QMessageBox::Yes);
            ui->pwdEdit->clear();
            ui->pwdEdit->setFocus();
            qCritical() << "json error!";
            return;
        }

        QJsonObject rootObj = jsonDoc.object();
        if(!rootObj.contains("code"))
        {
            QMessageBox::warning(this, tr("提示"), tr("登录失败"), QMessageBox::Yes);
            ui->pwdEdit->clear();
            ui->pwdEdit->setFocus();
            qCritical() << "login fail";
            return;
        }
        int code = rootObj.value("code").toInt();
        QString message = rootObj.value("message").toString();
        if(code)
        {
            QMessageBox::warning(this, tr("提示"), message, QMessageBox::Yes);
            ui->pwdEdit->clear();
            ui->pwdEdit->setFocus();
            qInfo() << message;
            return;
        }

        QString area("");
        int     userId = 0;
        int     areaId = 0;
        if(!rootObj.contains("data"))
        {
            qInfo() << "json no data";
        }
        else
        {
            QJsonArray dataArray = rootObj.value("data").toArray();
            QJsonObject data     = dataArray.at(0).toObject();
            if(data.contains("Id"))
            {
                userId = data.value("Id").toInt();
            }
            if(data.contains("AreaId"))
            {
                areaId = data.value("AreaId").toInt();
            }
            if(data.contains("AreaName"))
            {
                area = data.value("AreaName").toString();
            }
        }

        QString username = ui->userEdit->text();
        QString pwd      = ui->pwdEdit->text();
        int     status   = ui->pwdCheckBox->isChecked();
        User user(username, pwd, userId, areaId, area, status);
        bool    exist    = user.existUser();
        if(exist)
        {

            if(!user.updateUser())
            {
                qCritical() << QObject::tr("更新user失败");
            }
        }
        else
        {
            if(!user.insertUser())
            {
                qCritical() << QObject::tr("插入user失败");
            }
        }
        QMessageBox::information(this, tr("提示"), tr("登录成功"), QMessageBox::Yes);
        qInfo() << "login success";

        QDialog::accept();
    });
}
