#include "record.h"
#include "qlog.h"
#include "ui_record.h"

Record::Record(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Record)
{
    ui->setupUi(this);
    ui->recordTableWidget->setColumnWidth(0, 30);
    ui->recordTableWidget->setColumnWidth(1, 360);
    ui->recordTableWidget->setColumnWidth(2, 160);
    ui->recordTableWidget->setColumnWidth(3, 140);
    ui->recordTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->recordTableWidget->verticalHeader()->setHidden(true);
    ui->recordTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    recordList = UploadRecord::getAllRecord();
    if(recordList)
    {
        for(int i = 0; i < recordList->size(); ++i)
        {
            addWidgetItem(recordList->at(i));
        }
    }
    connect(ui->recordTableWidget, SIGNAL(cellClicked(int, int)), this, SLOT(change_chooseBtn(int, int)));
    connect(ui->recordTableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(select_all(int)));
}

Record::~Record()
{
    delete ui;
}

void Record::addWidgetItem(UploadRecord *record)
{
    int rowCount = ui->recordTableWidget->rowCount();
    ui->recordTableWidget->insertRow(rowCount);
    ui->recordTableWidget->setItem(rowCount, 0, new QTableWidgetItem(QIcon(":/new/icon/choose.png"), nullptr));
    ui->recordTableWidget->setItem(rowCount, 1, new QTableWidgetItem(record->getFilename()));
    ui->recordTableWidget->setItem(rowCount, 2, new QTableWidgetItem(record->getTime()));
    ui->recordTableWidget->setItem(rowCount, 3, new QTableWidgetItem(record->getMessage()));
}

void Record::change_chooseBtn(int row, int col)
{
    if(0 == col)
    {
        QTableWidgetItem *item = ui->recordTableWidget->item(row, col);
        if(item)
        {
            if(recordList->at(row)->getStatus())
            {
                item->setIcon(QIcon(":/new/icon/choose.png"));
                recordList->at(row)->setStatus(0);
            }
            else
            {
                item->setIcon(QIcon(":/new/icon/haschoose.png"));
                recordList->at(row)->setStatus(1);
            }
        }
    }
}

void Record::on_pushButton_clicked()
{
    int rowCount = ui->recordTableWidget->rowCount();
    for(int i = rowCount - 1; i >= 0; --i)
    {
        if(recordList->at(i)->getStatus())
        {
            ui->recordTableWidget->removeRow(i);
            recordList->at(i)->deleteRecord();
            recordList->removeOne(recordList->at(i));
        }
    }
}

void Record::on_pushButton_2_clicked()
{
    int rowCount = ui->recordTableWidget->rowCount();
    for(int i = rowCount - 1; i >= 0; --i)
    {
        if(recordList->at(i)->getStatus())
        {
            QTableWidgetItem *item = ui->recordTableWidget->item(i, 0);
            item->setIcon(QIcon(":/new/icon/choose.png"));
            recordList->at(i)->setStatus(0);
        }
    }
}

void Record::select_all(int col)
{
    if(0 == col)
    {
        int rowCount = ui->recordTableWidget->rowCount();
        for(int i = rowCount - 1; i >= 0; --i)
        {
            if(!recordList->at(i)->getStatus())
            {
                QTableWidgetItem *item = ui->recordTableWidget->item(i, 0);
                item->setIcon(QIcon(":/new/icon/haschoose.png"));
                recordList->at(i)->setStatus(1);
            }
        }
    }
}
