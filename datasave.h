#ifndef DATASAVE_H
#define DATASAVE_H
/*
This class is design to export data, in txt, from the ploting area
*/

#include <QObject>
#include <qcustomplot.h>
#include <QDebug>

class DataSave
{
public:
    DataSave();
    ~DataSave();

    void savedata(const QString &pathDirectory, const QVector<QString> &graphName, const QStringList &fileInfon, const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange);

private:
    QString createFileString(const QVector<QString> &graphName, const QStringList &fileInfon, const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange);
    QString createInfoString(const QStringList &fileInfon);
    QString createGraphNameString(const QVector<QString> &graphName);
    QString createGraphDataString(const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange);

};

#endif // DATASAVE_H
