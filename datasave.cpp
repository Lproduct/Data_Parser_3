#include "datasave.h"


DataSave::DataSave()
{

}

DataSave::~DataSave()
{

}

void DataSave::savedata(const QString &pathDirectory, const QVector<QString> &graphName, const QStringList &fileInfon, const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange)
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
    fileString = createFileString(graphName, fileInfon, graphData, keyRange);

    /* Write the line to the file */
    outStream << fileString;

    /* Close the file */
    outputFile.close();
    //return 0;
}

QString DataSave::createFileString(const QVector<QString> &graphName, const QStringList &fileInfon, const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange)
{
    //create info string
    QString stringInfo;
    stringInfo  = createInfoString(fileInfon);

    QString stringGraphName;
    stringGraphName = createGraphNameString(graphName);

    QString stringGraphData;
    stringGraphData = createGraphDataString(graphData, keyRange);

    QString stringReturn;
    stringReturn = stringInfo + stringGraphName + stringGraphData;

    return stringReturn;
}

QString DataSave::createInfoString(const QStringList &fileInfon)
{
    QString stringReturn;
    stringReturn += fileInfon.at(1) + "\r\n";
    stringReturn += "Date :\t" + fileInfon.at(2) + "\r\n";
    stringReturn += "Heure :\t" + fileInfon.at(3) + "\r\n";
    stringReturn += "Nom Opérateur :\t" + fileInfon.at(4) + "\r\n";
    stringReturn += "Nom du Site :\t" + fileInfon.at(5) + "\r\n";
    stringReturn += "Nom de l'essai :\t" + fileInfon.at(6) + "\r\n";
    stringReturn += "Commentaires :\t" + fileInfon.at(7) + "\r\n";
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
        stringReturn += graphName.at(i) + "\t";
    }
    stringReturn += graphName.at(graphName.size()-1) + "\r\n";

    return stringReturn;
}

QString DataSave::createGraphDataString(const QVector<QVector<QCPData> > &graphData, const QVector<double> &keyRange)
{
    QString stringReturn;

    QVector<QVector<QString> > tabString;
    QVector<QString> tabShift(keyRange.size());
    tabShift.fill("\t");
    QVector<QString> tabLineFeed(keyRange.size());
    tabLineFeed.fill("\r\n");

    //Copy and convert to string data time value
    QVector<QString> tabKey;
    for (int i(0); i<keyRange.size(); i++)
    {
        tabKey.push_back(QString::number(keyRange.at(i)));
    }
    tabString.push_back(tabKey);

    for (int i(0); i<graphData.size(); i++)
    {
        QVector<QString> tabValue(keyRange.size());
        tabValue.fill("0");
        for (int j(0); j<keyRange.size(); j++)
        {
            if (graphData.at(i).at(j).key == keyRange.at(j))
            {
                tabValue[j] = QString::number(graphData.at(i).at(j).value);
            }
            else
            {
               tabValue[j] = "0";
            }
        }
        tabString.push_back(tabValue);
    }


    /*for (int i(0); i<keyRange.size(); i++)
    {
        stringReturn += QString::number(keyRange.at(i));
        stringReturn += "\t";
        for (int j(0); j< graphData.size(); j++)
        {
            if (graphData.at(j).value(i).key == keyRange.at(i))
            {
                stringReturn += QString::number(graphData.at(j).value(i).value);
                if (j == graphData.size()-1)
                {
                    stringReturn += "";
                }
                else
                {
                    stringReturn += "\t";
                }
            }
            else
            {
                if (j == graphData.size()-1)
                {
                    stringReturn += "0";
                }
                else
                {
                    stringReturn += "\t";
                }
            }
        }

        stringReturn += "\r\n";
    }*/

    return stringReturn;
}


