#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QDialog *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_2_clicked();

    void on_loginBtn_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
