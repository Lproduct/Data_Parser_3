#ifndef TABDATA_H
#define TABDATA_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class TabData;
}

class TabData : public QDialog
{
    Q_OBJECT

public:
    explicit TabData(QWidget *parent = 0);
    ~TabData();
    void setInfo(const QString &curveName);
    void setData(const QVector<QCPData> &graphData, const int &timeAbsisState, const double &unit);

public slots:
    void emitSignal();

signals:
    void emitData(QVector<QVector<double> > data);

private:
    QString convertdoubleIntoTime(const double &value, const double &unit);
    Ui::TabData *ui;
    int m_nbCurve;
    QVector<double> m_tabkey;
};

#endif // TABDATA_H
