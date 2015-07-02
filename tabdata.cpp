#include "tabdata.h"
#include "ui_tabdata.h"
#include "qcustomplot.h"
#include <time.h>

TabData::TabData(QWidget *parent) : QDialog(parent), ui(new Ui::TabData)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/tabicon.png"));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(emitSignal()));
}

TabData::~TabData()
{
    delete ui;
}

void TabData::setInfo(const QString &curveName)
{
    this->setWindowTitle(curveName);
}

void TabData::setData(const QVector<QCPData> &graphData, const int &timeAbsisState, const double &unit)
{
    ui->tableWidget->setRowCount(graphData.size());
    ui->tableWidget->setColumnCount(2);

    if (timeAbsisState == 0)
    {
        for(int i(0); i<graphData.size(); i++)
        {
            m_tabkey.push_back(graphData.at(i).key);

            QTableWidgetItem *itemKey = new QTableWidgetItem(QString::number(graphData.at(i).key),QTableWidgetItem::Type);
            itemKey->setFlags(itemKey->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem( i, 0, itemKey);

            ui->tableWidget->setItem( i, 1, new QTableWidgetItem(QString::number(graphData.at(i).value)));
        }
    }
    else if (timeAbsisState == 2)
    {
        for(int i(0); i<graphData.size(); i++)
        {
            m_tabkey.push_back(graphData.at(i).key);

            QTableWidgetItem *itemKey = new QTableWidgetItem(convertdoubleIntoTime(graphData.at(i).key, unit),QTableWidgetItem::Type);
            itemKey->setFlags(itemKey->flags() ^ Qt::ItemIsEditable);
            ui->tableWidget->setItem( i, 0, itemKey);

            ui->tableWidget->setItem( i, 1, new QTableWidgetItem(QString::number(graphData.at(i).value)));
        }
    }
}

QString TabData::convertdoubleIntoTime(const double &value, const double &unit)
{
    double timeS(value * unit);
    QString stringReturn;

    //Calculate days
    double dayS(timeS /(3600*24));
    double hourS((dayS - (int) dayS)*24);
    double minS((hourS - (int) hourS)*60);
    double secS((minS - (int) minS)*60);
    double msecS((secS - (int) secS)*100);

    stringReturn += QString::number((int) hourS);
    stringReturn += ":";
    stringReturn += QString::number((int) minS);
    stringReturn += ":";
    stringReturn += QString::number((int) secS);
    stringReturn += ":";
    stringReturn += QString::number((int) msecS);

    return stringReturn;
}

void TabData::emitSignal()
{
    //QVector<double> tabKey;
    QVector<double> tabValue;
    for (int i(0); i<ui->tableWidget->rowCount(); i++)
    {
        //tabKey.push_back(ui->tableWidget->item(i, 0)->text().toDouble());
        tabValue.push_back(ui->tableWidget->item(i, 1)->text().toDouble());
    }

    QVector<QVector<double> > tabEmit;
    tabEmit.push_back(m_tabkey);
    tabEmit.push_back(tabValue);

    emitData(tabEmit);
}

void emitData(QVector<QVector<double> > data)
{
    emit data;
}

