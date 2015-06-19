#include "tabdata.h"
#include "ui_tabdata.h"
#include "qcustomplot.h"

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

void TabData::setData(const QVector<QCPData> &graphData)
{
    ui->tableWidget->setRowCount(graphData.size());
    ui->tableWidget->setColumnCount(2);

    for(int i(0); i<graphData.size(); i++)
    {
        QTableWidgetItem *itemKey = new QTableWidgetItem(QString::number(graphData.at(i).key),QTableWidgetItem::Type);
        itemKey->setFlags(itemKey->flags() ^ Qt::ItemIsEditable);
        ui->tableWidget->setItem( i, 0, itemKey);

        ui->tableWidget->setItem( i, 1, new QTableWidgetItem(QString::number(graphData.at(i).value)));
    }
}

void TabData::emitSignal()
{
    QVector<double> tabKey;
    QVector<double> tabValue;
    for (int i(0); i<ui->tableWidget->rowCount(); i++)
    {
        tabKey.push_back(ui->tableWidget->item(i, 0)->text().toDouble());
        tabValue.push_back(ui->tableWidget->item(i, 1)->text().toDouble());
    }

    QVector<QVector<double> > tabEmit;
    tabEmit.push_back(tabKey);
    tabEmit.push_back(tabValue);

    emitData(tabEmit);
}

void emitData(QVector<QVector<double> > data)
{
    emit data;
}

