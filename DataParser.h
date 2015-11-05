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

    bool getReadFileStatus();
    QString getReadFileErrorMessage();
    bool getTabParsingStatus();
    bool getInfoFileStatus();
    QString getInfoFileMessage();

private:
    int findWordInTab(const QStringList& tab, const QString& word);
    QString fileTxtInfo(const QString& info);

    QString m_fileName;
    QStringList tabData;
    QStringList infoFile;
    QStringList header;
    QVector<QVector<double> > tabNumericData;

    bool readFileStatus;
    QString readFileErrorMessage;

    bool infoFileStatus;
    QString infoFileStatusMessage;

    bool tabParsingStatus;
};

#endif // DATAPARSER_H
