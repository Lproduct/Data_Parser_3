#include "DataParser.h"

DataParser::DataParser(const QString &fileName): m_fileName(fileName)
{
    QFile file(m_fileName);
    file.open(QFile::ReadOnly | QFile::Text);
    /*if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Data Parser"), tr("Cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
        //return false;
    }*/

    //Read all the data from the file
    QTextStream in(&file);
    QString dataTxt(in.readAll());

    // Read the whole txt file and create a list which contain every sentences
    tabData = dataTxt.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

    //file close
    file.close();
}

DataParser::~DataParser()
{

}

QStringList DataParser::getInfoFile()
{
    // Looking for word position
    QString wordDate("Date :");
    QString wordHeure("Heure :");
    QString wordNomOperateur("Nom Opérateur :");
    QString wordNomSite("Nom du Site :");
    QString wordNomEssai("Nom de l'essai :");
    QString wordCommentaires("Commentaires :");

    //int fichierPosList = findWordInTab(tabData, wordFichier);
    int datePosList = findWordInTab(tabData, wordDate);
    int heurePosList = findWordInTab(tabData, wordHeure);
    int nomOperateurPosList = findWordInTab(tabData, wordNomOperateur);
    int sitePosList = findWordInTab(tabData, wordNomSite);
    int essaiPosList = findWordInTab(tabData, wordNomEssai);
    int commentairesPosList = findWordInTab(tabData, wordCommentaires);

    QString fichier(tabData.at(0));
    QString date(fileTxtInfo(tabData.at(datePosList)));
    QString heure(fileTxtInfo(tabData.at(heurePosList)));
    QString nomOperateur(fileTxtInfo(tabData.at(nomOperateurPosList)));
    QString nomSite(fileTxtInfo(tabData.at(sitePosList)));
    QString nomEssai(fileTxtInfo(tabData.at(essaiPosList)));
    QString commentaires(fileTxtInfo(tabData.at(commentairesPosList)));

    infoFile.append(m_fileName);
    infoFile.append(fichier);
    infoFile.append(date);
    infoFile.append(heure);
    infoFile.append(nomOperateur);
    infoFile.append(nomSite);
    infoFile.append(nomEssai);
    infoFile.append(commentaires);

    return infoFile;
}

QStringList DataParser::getHeader()
{
    //looking for header(curve name)
    QString wordTemps("Temps");
    int tempsPosList = findWordInTab(tabData, wordTemps);
    QString cell(tabData.at(tempsPosList));
    header = cell.split(QRegExp("\t"));

    return header;
}

QVector<QVector<double> > DataParser::getTab()
{
    QString wordTemps("Temps");
    int tempsPosList = findWordInTab(tabData, wordTemps);

    //transform tab data into numeric tab. Two fisrt row represent the number of column and number
    //of row the other data organised as tab witch are stacked.
    double numericData(0);
    QString cellStrData;
    QStringList cellStrDataList;

    int nbColumn(header.size());
    long int nbRow(tabData.size());//- tempsPosList -2

    for(int i(0); i<=nbColumn-1; i++)
    {
        QVector<double> subTab;
        for(long int k(tempsPosList+1); k<=nbRow-1; k++)
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

int DataParser::findWordInTab(const QStringList& tab, const QString& word)
{
    // Return the position of a word in a QStringList
    bool wordIsFind(false);
    int wordPosLine = 0;
    while (wordIsFind == false)
    {
        QString cellControl(tab.at(wordPosLine));
        QStringList CellControlList(cellControl.split(QRegExp("\t")));
        wordIsFind = CellControlList.contains(word);
        wordPosLine++;
    }
    return wordPosLine-1;
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

