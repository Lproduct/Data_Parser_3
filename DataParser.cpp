#include "DataParser.h"

DataParser::DataParser(const QString &fileName): m_fileName(fileName)
{
    readFileStatus = false;
    readFileErrorMessage = "";
    tabParsingStatus = false;
    QFile file(m_fileName);
    //file.open(QFile::ReadOnly | QFile::Text);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        //emit errorFileRead(tr("The program cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
        //emit errorFileReadState(false);
        //QMessageBox::warning(this, tr("Data Parser"), tr("Cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
        //return false;
        readFileStatus = false;
        readFileErrorMessage = QString("The program cannot read file %1:\n%2").arg(fileName).arg(file.errorString());
    }
    else
    {
        //emit errorFileReadState(true);
        readFileStatus = true;
        readFileErrorMessage = "";
        //Read all the data from the file
        QTextStream in(&file);
        QString dataTxt(in.readAll());

        // Read the whole txt file and create a list which contain every sentences
        tabData = dataTxt.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

        //file close
        file.close();
    }
}

DataParser::~DataParser()
{

}

QStringList DataParser::getInfoFile()
{
    infoFileStatus = false;

    //get file info
    QString fichier(tabData.at(0));
    infoFile.append(m_fileName);
    infoFile.append(fichier);
    //Get Date info
    QString wordDate("Date :");
    int datePosList = findWordInTab(tabData, wordDate);
    if(datePosList == -1)
    {
        infoFileStatus = true;
        infoFileStatusMessage += "Date";
        infoFileStatusMessage += "\n";
        infoFile.append("");
    }
    else
    {
        QString date(fileTxtInfo(tabData.at(datePosList)));
        infoFile.append(date);
    }
    //get hour info
    QString wordHeure("Heure :");
    int heurePosList = findWordInTab(tabData, wordHeure);
    if(heurePosList == -1)
    {
        infoFileStatus = true;
        infoFileStatusMessage += "Hour";
        infoFileStatusMessage += "\n";
        infoFile.append("");
    }
    else
    {
        QString heure(fileTxtInfo(tabData.at(heurePosList)));
        infoFile.append(heure);
    }
    //get operator name
    QString wordNomOperateur("Nom Opérateur :");
    int nomOperateurPosList = findWordInTab(tabData, wordNomOperateur);
    if(nomOperateurPosList == -1)
    {
        infoFileStatus = true;
        infoFileStatusMessage += "Operator name";
        infoFileStatusMessage += "\n";
        infoFile.append("");
    }
    else
    {
        QString nomOperateur(fileTxtInfo(tabData.at(nomOperateurPosList)));
        infoFile.append(nomOperateur);
    }
    //get area info
    QString wordNomSite("Nom du Site :");
    int sitePosList = findWordInTab(tabData, wordNomSite);
    if(sitePosList == -1)
    {
        infoFileStatus = true;
        infoFileStatusMessage += "Operator";
        infoFileStatusMessage += "\n";
        infoFile.append("");
    }
    else
    {
        QString nomSite(fileTxtInfo(tabData.at(sitePosList)));
        infoFile.append(nomSite);
    }
    //get sample info
    QString wordNomEssai("Nom de l'essai :");
    int essaiPosList = findWordInTab(tabData, wordNomEssai);
    if(essaiPosList == -1)
    {
        infoFileStatus = true;
        infoFileStatusMessage += "Sample";
        infoFileStatusMessage += "\n";
        infoFile.append("");
    }
    else
    {
        QString nomEssai(fileTxtInfo(tabData.at(essaiPosList)));
        infoFile.append(nomEssai);
    }
    //get comment info
    QString wordCommentaires("Commentaires :");
    int commentairesPosList = findWordInTab(tabData, wordCommentaires);
    if(commentairesPosList == -1)
    {
        infoFileStatus = true;
        infoFileStatusMessage += "Comment";
        infoFileStatusMessage += "\n";
        infoFile.append("");
    }
    else
    {
        QString commentaires(fileTxtInfo(tabData.at(commentairesPosList)));
        infoFile.append(commentaires);
    }

    return infoFile;
}

bool DataParser::getInfoFileStatus()
{
    return infoFileStatus;
}

QString DataParser::getInfoFileMessage()
{
    return infoFileStatusMessage;
}

QStringList DataParser::getHeader()
{
    //looking for header(curve name)
    /*QString wordTemps("Temps");
    int tempsPosList = findWordInTab(tabData, wordTemps);
    QString cell(tabData.at(tempsPosList));
    header = cell.split(QRegExp("\t"));*/

    return header;
}

QVector<QVector<double> > DataParser::getTab()
{
    QString wordTemps("Temps");
    int tempsPosList = findWordInTab(tabData, wordTemps);
    if (tempsPosList == -1)
    {
        tabParsingStatus = false;
        return tabNumericData;
    }
    else
    {
        tabParsingStatus = true;
        //transform tab data into numeric tab. Two fisrt row represent the number of column and number
        //of row the other data organised as tab witch are stacked.
        double numericData(0);
        QString cellStrData;
        QStringList cellStrDataList;

        //Get header
        QString cell(tabData.at(tempsPosList));
        header = cell.split(QRegExp("\t"));

        int nbColumn(header.size());
        long int nbRow(tabData.size());

        for(int i(0); i< nbColumn; i++)
        {
            QVector<double> subTab;
            for(long int k(tempsPosList+1); k< nbRow; k++)
            {
                cellStrData = tabData.at(k);
                cellStrDataList = cellStrData.split(QRegExp("\t"));
                numericData = cellStrDataList[i].toDouble();
                subTab.push_back(numericData);
            }
            tabNumericData.push_back(subTab);
        }
        return tabNumericData;
    }

}

int DataParser::findWordInTab(const QStringList& tab, const QString& word)
{
    int wordPosLine = -1;
    for(int i(0); i < tab.size(); i++)
    {
        QString cellControl(tab.at(i));
        QStringList CellControlList(cellControl.split(QRegExp("\t")));
        if(CellControlList.contains(word) == true)
        {
            wordPosLine = i;
            break;
        }
    }
    return wordPosLine;

}

QString DataParser::fileTxtInfo(const QString& info)
{
    //Permit to parse information of the info file header, if there is no information the string is null
    QStringList infoList = info.split(QRegExp("\t"));
    if (infoList.size() == 1)
    {
        QString infoTr(" ");
        return infoTr;
    }
    else
    {
        QString infoTr2(infoList.at(1));
        return infoTr2;
    }
}

bool DataParser::getReadFileStatus()
{
    return readFileStatus;
}

QString DataParser::getReadFileErrorMessage()
{
    return readFileErrorMessage;
}

bool DataParser::getTabParsingStatus()
{
    return tabParsingStatus;
}
