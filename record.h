#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
#include <QLabel>
#include <QToolButton>
#include <QGridLayout>
#include <QList>
#include <QTableWidgetItem>
#include "uploadrecord.h"

namespace Ui {
class Record;
}

class Record : public QDialog
{
    Q_OBJECT

public:
    explicit Record(QWidget *parent = nullptr);
    ~Record();
    void addWidgetItem(UploadRecord *record);

private slots:
    void change_chooseBtn(int, int);

    void select_all(int);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Record  *ui;
    QList<UploadRecord*> *recordList;
};

#endif // RECORD_H
