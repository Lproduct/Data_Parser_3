#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <QtWidgets>

class DataParser
{
public:
    DataParser(const QString &fileName);
    ~DataParser();

    QStringList getInfoFile();
    QStringList getHeader();
    QVector<QVector<double> > getTab();

private:
    int findWordInTab(const QStringList& tab, const QString& word);
    QString fileTxtInfo(const QString& info);

    QString m_fileName;
    QStringList tabData;
    QStringList infoFile;
    QStringList header;
    QVector<QVector<double> > tabNumericData;


};

#endif // DATAPARSER_H
