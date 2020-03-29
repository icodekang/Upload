#include "uploadthread.h"
#include "seting.h"
#include "qlog.h"
#include "tool.h"
#include "task.h"
#include "config.h"
#include "user.h"
#include "alarm.h"
#include "uploadrecord.h"
#include "testitem.h"
#include "gumbo_query/Document.h"
#include "gumbo_query/Node.h"

UploadThread::UploadThread(QObject *parent):QThread(parent)
{

}

void UploadThread::run()
{
    while(true)
    {
        Seting *seting      = Seting::getSeting();
        QString dirPath        = seting->getPath();
        unsigned long times = QString::number(seting->getTimes()).toULong();
        QDir    dir(dirPath);
        if(!dir.exists())
        {
            sleep(times);
            continue;
        }
        QStringList filters = Tool::getFileNames(dirPath);
        qDebug() << "start parse html";
        int successCount = 0;
        int hasSucNum = 0;
        for(int i = 0; i < filters.size(); ++i)
        {
            QString filename = filters.at(i);
            qDebug() << "upload " << filename;
            Task *task = Task::getTask(filename);
            if(task)
            {
                hasSucNum++;
                continue;
            }

            QString filePath = QString("%1/%2").arg(dirPath).arg(filename);
            QFile file(filePath);
            if(!file.open(QFile::ReadOnly | QFile::Text))
            {
                UploadRecord record(filename, tr("读取体检报告失败"), "", 0);
                if(UploadRecord::getRecord(filename))
                {
                    record.updateRecord();
                }
                else
                {
                    record.insertRecord();
                }
                qCritical() << QString("read %1 fail").arg(filename);
                continue;
            }

            QString reqData;
            QString reqAreaId;
            QString reqNumber;
            QString reqPerson;
            QString reqDetail;

            //AreaId
            User *user = User::getUser();
            QString areaId = QString::number(user->getAreaId());

            QByteArray html = file.readAll();
            CDocument doc;
            doc.parse(html.toStdString());
            if(doc.find("FONT").nodeNum() < 2)
            {
                 UploadRecord record(filename, tr("格式错误"), "", 0);
                 if(UploadRecord::getRecord(filename))
                 {
                     record.updateRecord();
                 }
                 else
                 {
                     record.insertRecord();
                 }
                 qCritical() << "upload docment font format error";
                 continue;
            }
            CNode node00 = doc.find("FONT").nodeAt(0);
            QString title = QString::fromStdString(node00.text());
            CNode node01 = doc.find("FONT").nodeAt(1);
            QString nameAndNum = QString::fromStdString(node01.text());
            int numIndex = Tool::getStrNumIndex(nameAndNum);
            if(-1 == numIndex)
            {
                UploadRecord record(filename, tr("格式错误"), "", 0);
                if(UploadRecord::getRecord(filename))
                {
                    record.updateRecord();
                }
                else
                {
                    record.insertRecord();
                }
                qCritical() << "upload docment font format error";
                continue;
            }
            QString name = nameAndNum.mid(0, numIndex);
            QString phone = nameAndNum.right(nameAndNum.length() - numIndex);
            QString pwd  = phone.right(4);
            if(doc.find("TBODY").nodeNum() < 3)
            {
                 UploadRecord record(filename, tr("格式错误"), "", 0);
                 if(UploadRecord::getRecord(filename))
                 {
                     record.updateRecord();
                 }
                 else
                 {
                     record.insertRecord();
                 }
                 qCritical() << "upload docment tbody format error";
                 continue;
            }
            CNode bodyNode1 = doc.find("TBODY").nodeAt(0);
            if(bodyNode1.find("FONT").nodeNum() < 6)
            {
                 UploadRecord record(filename, tr("格式错误"), "", 0);
                 if(UploadRecord::getRecord(filename))
                 {
                     record.updateRecord();
                 }
                 else
                 {
                     record.insertRecord();
                 }
                 qCritical() << "upload docment font format error";
                 continue;
            }
            CNode node10 = bodyNode1.find("FONT").nodeAt(0);
            QString sexStr = QString::fromStdString(node10.text());
            QString sex = sexStr.section(' ', 1);
            CNode node11 = bodyNode1.find("FONT").nodeAt(1);
            QString ageStr = QString::fromStdString(node11.text());
            QString age = ageStr.section(' ', 1);
            CNode node12 = bodyNode1.find("FONT").nodeAt(2);
            QString weightStr = QString::fromStdString(node12.text());
            QString weight = weightStr.section(' ', 1);
            CNode node13 = bodyNode1.find("FONT").nodeAt(3);
            QString pulseStr = QString::fromStdString(node13.text());
            QString pulse = pulseStr.section(' ', 1);
            CNode node14 = bodyNode1.find("FONT").nodeAt(4);
            QString breathStr = QString::fromStdString(node14.text());
            QString breath = breathStr.section(' ', 1);
            CNode node15 = bodyNode1.find("FONT").nodeAt(5);
            QString atmosStr = QString::fromStdString(node15.text());
            QString atmos = atmosStr.section(' ', 1);
            QString curtime = Tool::getCurTime();
            QString userId = QString::number(user->getUserId());
            delete user;

            CNode bodyNode2 = doc.find("TBODY").nodeAt(1);
            if(bodyNode2.find("p").nodeNum() < 7)
            {
                 UploadRecord record(filename, tr("格式错误"), "", 0);
                 if(UploadRecord::getRecord(filename))
                 {
                     record.updateRecord();
                 }
                 else
                 {
                     record.insertRecord();
                 }
                 qCritical() << "upload docment p format error";
                 continue;
            }
            CNode node20 = bodyNode2.find("P").nodeAt(0);
            QString lcaStr = QString::fromStdString(node20.text());
            QString lca = lcaStr.section(' ', 1);
            CNode node21 = bodyNode2.find("P").nodeAt(1);
            QString rcaStr = QString::fromStdString(node21.text());
            QString rca = rcaStr.section(' ', 1);
            CNode node22 = bodyNode2.find("P").nodeAt(2);
            QString lacStr = QString::fromStdString(node22.text());
            QString lac = lacStr.section(' ', 1);
            CNode node23 = bodyNode2.find("P").nodeAt(3);
            QString racStr = QString::fromStdString(node23.text());
            QString rac = racStr.section(' ', 1);
            CNode node24 = bodyNode2.find("P").nodeAt(4);
            QString abdStr = QString::fromStdString(node24.text());
            QString abd = abdStr.section(' ', 1);
            CNode node25 = bodyNode2.find("P").nodeAt(5);
            QString paraSum = QString::fromStdString(node25.text());
            CNode node26 = bodyNode2.find("P").nodeAt(6);
            QString stable = QString::fromStdString(node26.text());
            QString remark = QString(html).section("<br>\n", 2, -3).remove(QRegExp("\\s")).remove(QRegExp("<br>"));

            QList<TestItem> test_item;
            QList<TestItem> test_blood;
            QList<TestItem> test_Electrolyte;
            QList<TestItem> test_coagulation;
            QList<TestItem> test_lung;
            QList<TestItem> test_oxygen;
            QList<TestItem> test_heart;
            QList<TestItem> test_lgs;
            QList<TestItem> test_kidney;
            QList<TestItem> test_Reproduction;
            QList<TestItem> test_endocrine;
            QList<TestItem> test_carbohydrate;
            QList<TestItem> test_lipid;
            QList<TestItem> test_emotion;
            QList<TestItem> test_brain;
            QList<TestItem> test_motion;
            QList<TestItem> test_immunity;
            QList<TestItem> test_skin;
            QList<TestItem> test_healthy;

            CNode bodyNode3 = doc.find("TBODY").nodeAt(2);
            if(bodyNode3.find("TR").nodeNum() < 144)
            {
                 UploadRecord record(filename, tr("格式错误"), "", 0);
                 if(UploadRecord::getRecord(filename))
                 {
                     record.updateRecord();
                 }
                 else
                 {
                     record.insertRecord();
                 }
                 qCritical() << "upload docment tr format error";
                 continue;
            }
            unsigned int trCount    = QString::number(bodyNode3.find("TR").nodeNum()).toUInt();
            unsigned int curTrCount = 2;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_item.append(item);
            }

            QString itemStr;
            for(int i = 0; i < test_item.size(); ++i)
            {
                QString item;
                if(i == test_item.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_item.at(i).id)
                                   .arg(test_item.at(i).value)
                                   .arg(test_item.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_item.at(i).id)
                                   .arg(test_item.at(i).value)
                                   .arg(test_item.at(i).status);
                }
                itemStr.append(item);
            }
            //qDebug() << itemStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_blood.append(item);
            }
            QString bloodStr;
            for(int i = 0; i < test_blood.size(); ++i)
            {
                QString item;
                if(i == test_blood.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_blood.at(i).id)
                                   .arg(test_blood.at(i).value)
                                   .arg(test_blood.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_blood.at(i).id)
                                   .arg(test_blood.at(i).value)
                                   .arg(test_blood.at(i).status);
                }
                bloodStr.append(item);
            }
            //qDebug() << bloodStr;

            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_Electrolyte.append(item);
            }
            QString electrolyteStr;
            for(int i = 0; i < test_Electrolyte.size(); ++i)
            {
                QString item;
                if(i == test_Electrolyte.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_Electrolyte.at(i).id)
                                   .arg(test_Electrolyte.at(i).value)
                                   .arg(test_Electrolyte.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_Electrolyte.at(i).id)
                                   .arg(test_Electrolyte.at(i).value)
                                   .arg(test_Electrolyte.at(i).status);
                }
                electrolyteStr.append(item);
            }
            //qDebug() << electrolyteStr;

            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_coagulation.append(item);
            }
            QString coagulationStr;
            for(int i = 0; i < test_coagulation.size(); ++i)
            {
                QString item;
                if(i == test_coagulation.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_coagulation.at(i).id)
                                   .arg(test_coagulation.at(i).value)
                                   .arg(test_coagulation.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_coagulation.at(i).id)
                                   .arg(test_coagulation.at(i).value)
                                   .arg(test_coagulation.at(i).status);
                }
                coagulationStr.append(item);
            }
            //qDebug() << coagulationStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_lung.append(item);
            }
            QString lungStr;
            for(int i = 0; i < test_lung.size(); ++i)
            {
                QString item;
                if(i == test_lung.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_lung.at(i).id)
                                   .arg(test_lung.at(i).value)
                                   .arg(test_lung.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_lung.at(i).id)
                                   .arg(test_lung.at(i).value)
                                   .arg(test_lung.at(i).status);
                }
                lungStr.append(item);
            }
            //qDebug() << lungStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_oxygen.append(item);
            }
            QString oxygenStr;
            for(int i = 0; i < test_oxygen.size(); ++i)
            {
                QString item;
                if(i == test_oxygen.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_oxygen.at(i).id)
                                   .arg(test_oxygen.at(i).value)
                                   .arg(test_oxygen.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_oxygen.at(i).id)
                                   .arg(test_oxygen.at(i).value)
                                   .arg(test_oxygen.at(i).status);
                }
                oxygenStr.append(item);
            }
            //qDebug() << oxygenStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_heart.append(item);
            }
            QString heartStr;
            for(int i = 0; i < test_heart.size(); ++i)
            {
                QString item;
                if(i == test_heart.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_heart.at(i).id)
                                   .arg(test_heart.at(i).value)
                                   .arg(test_heart.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_heart.at(i).id)
                                   .arg(test_heart.at(i).value)
                                   .arg(test_heart.at(i).status);
                }
                heartStr.append(item);
            }
            //qDebug() << heartStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_lgs.append(item);
            }
            QString lgsStr;
            for(int i = 0; i < test_lgs.size(); ++i)
            {
                QString item;
                if(i == test_lgs.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_lgs.at(i).id)
                                   .arg(test_lgs.at(i).value)
                                   .arg(test_lgs.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_lgs.at(i).id)
                                   .arg(test_lgs.at(i).value)
                                   .arg(test_lgs.at(i).status);
                }
                lgsStr.append(item);
            }
            //qDebug() << lgsStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_kidney.append(item);
            }
            QString kidneyStr;
            for(int i = 0; i < test_kidney.size(); ++i)
            {
                QString item;
                if(i == test_kidney.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_kidney.at(i).id)
                                   .arg(test_kidney.at(i).value)
                                   .arg(test_kidney.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_kidney.at(i).id)
                                   .arg(test_kidney.at(i).value)
                                   .arg(test_kidney.at(i).status);
                }
                kidneyStr.append(item);
            }
            //qDebug() << kidneyStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_Reproduction.append(item);
            }
            QString reproductionStr;
            for(int i = 0; i < test_Reproduction.size(); ++i)
            {
                QString item;
                if(i == test_Reproduction.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_Reproduction.at(i).id)
                                   .arg(test_Reproduction.at(i).value)
                                   .arg(test_Reproduction.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_Reproduction.at(i).id)
                                   .arg(test_Reproduction.at(i).value)
                                   .arg(test_Reproduction.at(i).status);
                }
                reproductionStr.append(item);
            }
            //qDebug() << reproductionStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_endocrine.append(item);
            }
            QString endocrineStr;
            for(int i = 0; i < test_endocrine.size(); ++i)
            {
                QString item;
                if(i == test_endocrine.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_endocrine.at(i).id)
                                   .arg(test_endocrine.at(i).value)
                                   .arg(test_endocrine.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_endocrine.at(i).id)
                                   .arg(test_endocrine.at(i).value)
                                   .arg(test_endocrine.at(i).status);
                }
                endocrineStr.append(item);
            }
            //qDebug() << endocrineStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_carbohydrate.append(item);
            }
            QString carbohydrateStr;
            for(int i = 0; i < test_carbohydrate.size(); ++i)
            {
                QString item;
                if(i == test_carbohydrate.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_carbohydrate.at(i).id)
                                   .arg(test_carbohydrate.at(i).value)
                                   .arg(test_carbohydrate.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_carbohydrate.at(i).id)
                                   .arg(test_carbohydrate.at(i).value)
                                   .arg(test_carbohydrate.at(i).status);
                }
                carbohydrateStr.append(item);
            }
            //qDebug() << carbohydrateStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_lipid.append(item);
            }
            QString lipidStr;
            for(int i = 0; i < test_lipid.size(); ++i)
            {
                QString item;
                if(i == test_lipid.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_lipid.at(i).id)
                                   .arg(test_lipid.at(i).value)
                                   .arg(test_lipid.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_lipid.at(i).id)
                                   .arg(test_lipid.at(i).value)
                                   .arg(test_lipid.at(i).status);
                }
                lipidStr.append(item);
            }
            //qDebug() << lipidStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_emotion.append(item);
            }
            QString emotionStr;
            for(int i = 0; i < test_emotion.size(); ++i)
            {
                QString item;
                if(i == test_emotion.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_emotion.at(i).id)
                                   .arg(test_emotion.at(i).value)
                                   .arg(test_emotion.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_emotion.at(i).id)
                                   .arg(test_emotion.at(i).value)
                                   .arg(test_emotion.at(i).status);
                }
                emotionStr.append(item);
            }
            //qDebug() << emotionStr;
            for(unsigned int i = curTrCount; i <= trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_brain.append(item);
            }
            QString brainStr;
            for(int i = 0; i < test_brain.size(); ++i)
            {
                QString item;
                if(i == test_brain.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_brain.at(i).id)
                                   .arg(test_brain.at(i).value)
                                   .arg(test_brain.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_brain.at(i).id)
                                   .arg(test_brain.at(i).value)
                                   .arg(test_brain.at(i).status);
                }
                brainStr.append(item);
            }
            //qDebug() << brainStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_motion.append(item);
            }
            QString motionStr;
            for(int i = 0; i < test_motion.size(); ++i)
            {
                QString item;
                if(i == test_motion.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_motion.at(i).id)
                                   .arg(test_motion.at(i).value)
                                   .arg(test_motion.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_motion.at(i).id)
                                   .arg(test_motion.at(i).value)
                                   .arg(test_motion.at(i).status);
                }
                motionStr.append(item);
            }
            //qDebug() << motionStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_immunity.append(item);
            }
            QString immunityStr;
            for(int i = 0; i < test_immunity.size(); ++i)
            {
                QString item;
                if(i == test_immunity.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_immunity.at(i).id)
                                   .arg(test_immunity.at(i).value)
                                   .arg(test_immunity.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_immunity.at(i).id)
                                   .arg(test_immunity.at(i).value)
                                   .arg(test_immunity.at(i).status);
                }
                immunityStr.append(item);
            }
            //qDebug() << immunityStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_skin.append(item);
            }
            QString skinStr;
            for(int i = 0; i < test_skin.size(); ++i)
            {
                QString item;
                if(i == test_skin.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_skin.at(i).id)
                                   .arg(test_skin.at(i).value)
                                   .arg(test_skin.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_skin.at(i).id)
                                   .arg(test_skin.at(i).value)
                                   .arg(test_skin.at(i).status);
                }
                skinStr.append(item);
            }
            //qDebug() << skinStr;
            for(unsigned int i = curTrCount; i < trCount; ++i)
            {
                CNode node_item = bodyNode3.find("TR").nodeAt(i);
                if(node_item.find("TD").nodeNum() != 5)
                {
                    curTrCount +=2;
                    break;
                }
                else
                {
                    curTrCount++;
                }
                CNode node_id   = node_item.find("FONT").nodeAt(1);
                CNode node_value = node_item.find("FONT").nodeAt(4);
                QString colorStr = QString::fromStdString(node_value.attribute("color"));
                TestItem item;
                item.id = QString::fromStdString(node_id.text());
                item.value = QString::fromStdString(node_value.text()).remove(QRegExp("\\s"));
                if(colorStr == "#000000")
                {
                    item.status = "0";
                }
                else if(colorStr == "#0000ff")
                {
                    item.status = "1";
                }
                else if(colorStr == "#ff0000")
                {
                    item.status = "2";
                }
                test_healthy.append(item);
            }
            QString healthyStr;
            for(int i = 0; i < test_healthy.size(); ++i)
            {
                QString item;
                if(i == test_healthy.size() - 1)
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   }")
                                   .arg(test_healthy.at(i).id)
                                   .arg(test_healthy.at(i).value)
                                   .arg(test_healthy.at(i).status);
                }
                else
                {
                    item = QString("{\
                                    'ReportItemID':'%1',\
                                    'ProjectValueFloat':'%2',\
                                    'Status':'%3'\
                                   },")
                                   .arg(test_healthy.at(i).id)
                                   .arg(test_healthy.at(i).value)
                                   .arg(test_healthy.at(i).status);
                }
                healthyStr.append(item);
            }
            //qDebug() << healthyStr;
            QUrl url(Config::getUploadUrl());
            QString uploadBody = QString("{\
                                       'AreaId':'%1',\
                                       'Member':{\
                                            'Name':'%2',\
                                            'Pwd':'%3',\
                                            'Sex':'%4',\
                                            'Age':'%5',\
                                            'PhoneNum':'%6',\
                                            'Pulse':'%7',\
                                            'Weight':'%8',\
                                            'BreathRate':'%9',\
                                            'Contact':'%10',\
                                            'Uptime':'%11',\
                                            'CreateBy':'%12'\
                                       },\
                                       'Report_Person':{\
                                            'ReportTitle':'%13',\
                                            'CheckupTime':'%14',\
                                            'AtmosPressure':'%15',\
                                            'Sex':'%16',\
                                            'Age':'%17',\
                                            'PhoneNum':'%18',\
                                            'Pulse':'%19',\
                                            'Weight':'%20',\
                                            'BreathRate':'%21',\
                                            'LCA':'%22',\
                                            'RCA':'%23',\
                                            'LAC':'%24',\
                                            'RAC':'%25',\
                                            'ABD':'%26',\
                                            'ParaSum':'%27',\
                                            'Stable':'%28',\
                                            'Remark':'%29'\
                                       },\
                                       'Report_Details':{\
                                            '1':[%30],\
                                            '2':[%31],\
                                            '3':[%32],\
                                            '4':[%33],\
                                            '5':[%34],\
                                            '6':[%35],\
                                            '7':[%36],\
                                            '8':[%37],\
                                            '9':[%38],\
                                            '10':[%39],\
                                            '11':[%40],\
                                            '12':[%41],\
                                            '13':[%42],\
                                            '14':[%43],\
                                            '15':[%44],\
                                            '16':[%45],\
                                            '17':[%46],\
                                            '18':[%47],\
                                            '19':[%48]\
                                       }}").arg(areaId)
                                           .arg(name)//codec->toUnicode(name.toUtf8()))
                                           .arg(pwd)
                                           .arg(sex)
                                           .arg(age)
                                           .arg(phone)
                                           .arg(pulse)
                                           .arg(weight)
                                           .arg(breath)
                                           .arg(phone)
                                           .arg(Tool::getCurTime())
                                           .arg(userId)
                                           .arg(title)//codec->toUnicode(title.toUtf8()))
                                           .arg(Tool::getCurTime())
                                           .arg(atmos)
                                           .arg(sex)
                                           .arg(age)
                                           .arg(phone)
                                           .arg(pulse)
                                           .arg(weight)
                                           .arg(breath)
                                           .arg(lca)
                                           .arg(rca)
                                           .arg(lac)
                                           .arg(rac)
                                           .arg(abd)
                                           .arg(paraSum)
                                           .arg(stable)
                                           .arg(remark)
                                           .arg(itemStr)
                                           .arg(bloodStr)
                                           .arg(electrolyteStr)
                                           .arg(coagulationStr)
                                           .arg(lungStr)
                                           .arg(oxygenStr)
                                           .arg(heartStr)
                                           .arg(lgsStr)
                                           .arg(kidneyStr)
                                           .arg(reproductionStr)
                                           .arg(endocrineStr)
                                           .arg(carbohydrateStr)
                                           .arg(lipidStr)
                                           .arg(emotionStr)
                                           .arg(brainStr)
                                           .arg(motionStr)
                                           .arg(immunityStr)
                                           .arg(skinStr)
                                           .arg(healthyStr);
            //qDebug() << uploadBody;
            QNetworkAccessManager manager;
            QEventLoop loop;
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
            connect(&manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
            QNetworkReply *reply = manager.post(request, uploadBody.toUtf8());
            loop.exec();
            QByteArray repData = reply->readAll();
            qDebug() << QString("data: ").append(QString(repData));

            QJsonParseError jsonError;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(repData, &jsonError));
            if(jsonError.error != QJsonParseError::NoError)
            {
                UploadRecord record(filename, tr("返回数据格式错误"), "", 0);
                if(UploadRecord::getRecord(filename))
                {
                    record.updateRecord();
                }
                else
                {
                    record.insertRecord();
                }
                qCritical() << QObject::tr("json error! ") << reply->errorString() << QString("%1 upload fail").arg(filename);
                continue;
            }

            QJsonObject rootObj = jsonDoc.object();
            if(!rootObj.contains("code"))
            {
                UploadRecord record(filename, tr("返回数据格式错误"), "", 0);
                if(UploadRecord::getRecord(filename))
                {
                    record.updateRecord();
                }
                else
                {
                    record.insertRecord();
                }
                qCritical() << QString("%1 upload fail, no code").arg(filename);
                continue;
            }
            int code = rootObj.value("code").toInt();
            QString message = rootObj.value("message").toString();
            if(code)
            {
                UploadRecord record(filename, message, "", 0);
                if(UploadRecord::getRecord(filename))
                {
                    record.updateRecord();
                }
                else
                {
                    record.insertRecord();
                }
                qCritical() << message  << QString("%1 upload fail").arg(filename);
            }
            else
            {
                task = new Task(filename);
                task->insertTask();
                delete task;
                successCount++;
                UploadRecord record(filename, tr("上传成功"), "", 1);
                if(UploadRecord::getRecord(filename))
                {
                    record.updateRecord();
                }
                else
                {
                    record.insertRecord();
                }
                qInfo() << QString("%1 upload success").arg(filename);
            }
        }
        qDebug() << "success: " << successCount << " fail: " << filters.size() - successCount - hasSucNum;
        Alarm alarm(successCount, filters.size() - successCount - hasSucNum, 1);
        alarm.updateAlarm();
        sleep(times * 60);
    }
}
