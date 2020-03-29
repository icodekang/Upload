#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QDir>
#include <QSystemTrayIcon>
#include <QAction>
#include <QTimer>
#include <QFileDialog>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include "uploadthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum StackWidgetType
{
    AcountWidget,
    UploadWidget,
    MessageWidget
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private slots:
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_uploadDetection();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_1_clicked();
    void on_checkUpload();
    void on_showRecord();

private:
    Ui::MainWindow  *ui;
    QSystemTrayIcon *systemTray;
    UploadThread    *ut;
    QTimer          *timer;
    QTimer          *recordTimer;
};
#endif // MAINWINDOW_H
