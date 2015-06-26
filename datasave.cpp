#include "datasave.h"


DataSave::DataSave()
{

}

DataSave::~DataSave()
{

}

void DataSave::savedata(const QString &pathDirectory, const QVector<QString> &graphName, const QStringList &fileInfon, const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange, const bool &checkBoxDBLinfo, const QString &infoDBL)
{
    /* Try and open a file for output */
    QFile outputFile(pathDirectory);
    outputFile.open(QIODevice::WriteOnly);

    /* Check it opened OK */
    if(!outputFile.isOpen()){
        //qDebug() << argv[0] << "- Error, unable to open" << outputFilename << "for output";
        //return 1;
    }

    /* Point a QTextStream object at the file */
    QTextStream outStream(&outputFile);

    //Create string to return
    QString fileString;
    fileString = createFileString(graphName, fileInfon, graphData, keyRange, checkBoxDBLinfo, infoDBL);

    /* Write the line to the file */
    outStream << fileString;

    /* Close the file */
    outputFile.close();
    //return 0;
}

QString DataSave::createFileString(const QVector<QString> &graphName, const QStringList &fileInfon, const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange, const bool &checkBoxDBLinfo, const QString &infoDBL)
{
    //create info string
    QString stringInfo;
    stringInfo  = createInfoString(fileInfon, checkBoxDBLinfo, infoDBL);

    QString stringGraphName;
    stringGraphName = createGraphNameString(graphName);

    QString stringGraphData;
    stringGraphData = createGraphDataString(graphData, keyRange);

    QString stringReturn;
    stringReturn = stringInfo + stringGraphName + stringGraphData;

    return stringReturn;
}

QString DataSave::createInfoString(const QStringList &fileInfon, const bool &checkBoxDBLinfo, const QString &infoDBL)
{
    QString stringReturn;
    stringReturn += fileInfon.at(1) + "\r\n";
    stringReturn += "Date :\t" + fileInfon.at(2) + "\r\n";
    stringReturn += "Heure :\t" + fileInfon.at(3) + "\r\n";
    stringReturn += "Nom Opérateur :\t" + fileInfon.at(4) + "\r\n";
    stringReturn += "Nom du Site :\t" + fileInfon.at(5) + "\r\n";
    stringReturn += "Nom de l'essai :\t" + fileInfon.at(6) + "\r\n";
    //stringReturn += "Commentaires :\t" + fileInfon.at(7) + "\r\n";
    if (checkBoxDBLinfo == true)
    {
        QString infoDBLString(infoDBL);
        infoDBLString.replace(QRegExp("\n"), " ");
        stringReturn += "Commentaires :\t" + fileInfon.at(7);
        stringReturn += infoDBLString;
        stringReturn += "\r\n";
    }
    else if (checkBoxDBLinfo == false)
    {
        stringReturn += "Commentaires :\t" + fileInfon.at(7) + "\r\n";
    }
    stringReturn += "\r\n";
    stringReturn += "\r\n";
    return stringReturn;
}

QString DataSave::createGraphNameString(const QVector<QString> &graphName)
{
    QString stringReturn;

    stringReturn += "Temps\t";

    for (int i(0); i< graphName.size() -1; i++)
    {
        QString stringTemp(graphName.at(i));
        stringTemp.replace(QRegExp("\n"), "");
        stringReturn += stringTemp + "\t";
    }

    QString stringTemp(graphName.at(graphName.size()-1));
    stringTemp.replace(QRegExp("\n"), "");
    stringReturn += stringTemp + "\r\n";

    return stringReturn;
}

QString DataSave::createGraphDataString(const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange)
{
    //Create a tab of value and string
    QVector<QVector<QString> > tabString;
    QVector<QString> tabShift(keyRange.size());
    tabShift.fill("\t");
    QVector<QString> tabLineFeed(keyRange.size());
    tabLineFeed.fill("\r\n");

    QVector<QString> tabKey;
    for (int i(0); i<keyRange.size(); i++)
    {
        tabKey.push_back(QString::number(keyRange.at(i)));
    }
    tabString.push_back(tabKey);
    tabString.push_back(tabShift);

    for (int i(0); i<graphData.size(); i++)
    {
        QVector<QString> tabValue(keyRange.size());
        tabValue.fill("0");
        int startKey(returnIndexOfKey(graphData.at(i).at(0).key, keyRange));
        int endKey(returnIndexOfKey(graphData.at(i).at(graphData.at(i).size()-1).key, keyRange));

        for (int j(startKey); j<=endKey; j++)
        {
            tabValue[j] = QString::number(graphData.at(i).at(j - startKey).value);
        }
        tabString.push_back(tabValue);
        if (i<graphData.size()-1)
        {
            tabString.push_back(tabShift);
        }
        else if( i == graphData.size()-1)
        {
            tabString.push_back(tabLineFeed);
        }
    }

    //create a string with the tab
    QString stringReturn;

    for(int i(0); i<tabString.at(0).size(); i++)
    {
        for(int j(0); j<tabString.size(); j++)
        {
            stringReturn += tabString.at(j).at(i);
        }
    }

    return stringReturn;
}

int DataSave::returnIndexOfKey(const double &value, const QVector<double> &tabKey)
{
    int ind(0);
    for (int i(0); i<tabKey.size(); i++)
    {
        if (value == tabKey.at(i))
        {
            break;
        }
        else
        {
            ind++;
        }
    }
    return ind;
}


