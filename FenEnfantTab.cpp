#include "FenEnfantTab.h"

FenEnfantTab::FenEnfantTab()
{
    //Info data
    fichier = new QLineEdit();
    fichier->setReadOnly(true);
    fichier->setFont(QFont("Courier"));

    date = new QLineEdit();
    date->setReadOnly(true);
    date->setFont(QFont("Courier"));

    heure = new QLineEdit();
    heure->setReadOnly(true);
    heure->setFont(QFont("Courier"));

    nomOperateur = new QLineEdit();
    nomOperateur->setReadOnly(true);
    nomOperateur->setFont(QFont("Courier"));

    nomSite = new QLineEdit();
    nomSite->setReadOnly(true);
    nomSite->setFont(QFont("Courier"));

    nomEssai = new QLineEdit();
    nomEssai->setReadOnly(true);
    nomEssai->setFont(QFont("Courier"));

    commentaires = new QLineEdit();
    commentaires->setReadOnly(true);
    commentaires->setFont(QFont("Courier"));

    QFormLayout *infoLayout = new QFormLayout;
    infoLayout->addRow("Fichier :", fichier);
    infoLayout->addRow("Date :", date);
    infoLayout->addRow("Heure :", heure);
    infoLayout->addRow("Nom operateur :", nomOperateur);
    infoLayout->addRow("Nom du site :", nomSite);
    infoLayout->addRow("Nom de l'essai :", nomEssai);
    infoLayout->addRow("Commentaires :", commentaires);

    //Tab Data
    table = new QTableWidget();
    tableItem = new QTableWidgetItem();
    table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addLayout(infoLayout);
    layoutPrincipal->addWidget(table);
    setLayout(layoutPrincipal);
    setMinimumHeight(550);
    setMinimumWidth(600);
}

bool FenEnfantTab::loadFileTab(const QString &fileName)
{
    //Open a file
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Data Parser"), tr("Cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
        return false;
    }

    setCurrentFile(fileName);

    //Read all the data from the file
    QTextStream in(&file);
    QString dataTxt(in.readAll());

    //Parsing and display data into a widget
    parsingDataIntoWidget(dataTxt);

    file.close();

    return true;
}

QVector<double> FenEnfantTab::loadNumericData(const QString &fileName)
{
    QFile file(fileName);
    file.open(QFile::ReadOnly | QFile::Text);

    //Read all the data from the file
    QTextStream in(&file);
    QString dataTxt(in.readAll());

    file.close();

    //Parsing data into numeric tab
    return parsingDataIntoTab(dataTxt);
}

int FenEnfantTab::findWordInTab(const QStringList& tab, const QString& word)
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

void FenEnfantTab::generateTabWidget(const QStringList& tab)
{
    QString wordTemps("Temps");
    int tempsPosList = findWordInTab(tab, wordTemps);

    //Generate a TabWidget from a tab with a given header position
    QString cell(tab.at(tempsPosList));
    QStringList cellData(cell.split(QRegExp("\t")));

    table->setRowCount(tab.size()-tempsPosList-1);
    table->setColumnCount(cellData.size());

    //Header label
    table->setHorizontalHeaderLabels(cellData);

    for ( int i(tempsPosList+1); i<=tab.size()-1; i++)
    {
        cell = tab.at(i);
        cellData = cell.split(QRegExp("\t"));

        for (int k(0); k<= cellData.size()-1; k++)
        {
            table->setItem(i-tempsPosList-1,k,new QTableWidgetItem(cellData.at(k)));
        }
    }
}

QString FenEnfantTab::fileTxtInfo(const QString& info)
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

void FenEnfantTab::generateInfoWidget(const QStringList& tab)
{
    // Looking for word position
    QString wordDate("Date :");
    QString wordHeure("Heure :");
    QString wordNomOperateur(QString::fromLocal8Bit("Nom Opérateur :"));
    QString wordNomSite("Nom du Site :");
    QString wordNomEssai("Nom de l'essai :");
    QString wordCommentaires("Commentaires :");

    //int fichierPosList = findWordInTab(tabData, wordFichier);
    int datePosList = findWordInTab(tab, wordDate);
    int heurePosList = findWordInTab(tab, wordHeure);
    int nomOperateurPosList = findWordInTab(tab, wordNomOperateur);
    int sitePosList = findWordInTab(tab, wordNomSite);
    int essaiPosList = findWordInTab(tab, wordNomEssai);
    int commentairesPosList = findWordInTab(tab, wordCommentaires);

    fichier->setText(tab.at(0));
    date->setText(fileTxtInfo(tab.at(datePosList)));
    heure->setText(fileTxtInfo(tab.at(heurePosList)));
    nomOperateur->setText(fileTxtInfo(tab.at(nomOperateurPosList)));
    nomSite->setText(fileTxtInfo(tab.at(sitePosList)));
    nomEssai->setText(fileTxtInfo(tab.at(essaiPosList)));
    commentaires->setText(fileTxtInfo(tab.at(commentairesPosList)));
}

void FenEnfantTab::genererWidget(const QStringList& tab)
{
    //Generate widget with info and tab data
    generateInfoWidget(tab);
    generateTabWidget(tab);
}

void FenEnfantTab::parsingDataIntoWidget(const QString dataTxt)
{
    // Read the whole txt file and create a list which contain every sentences
    QStringList tabData(dataTxt.split(QRegExp("[\r\n]"), QString::SkipEmptyParts));

    genererWidget(tabData);
}

void FenEnfantTab::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    //isUntitled = false;
    //document()->setModified(false);
    //setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString FenEnfantTab::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

QString FenEnfantTab::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QVector<double> FenEnfantTab::parsingDataIntoTab(const QString dataTxt)
{
    //Parsing data and generate a tab witch contain size, dimension and all numerical values
    // Read the whole txt file and create a list which contain every sentences
    QStringList tab(dataTxt.split(QRegExp("[\r\n]"), QString::SkipEmptyParts));

    QString wordTemps("Temps");
    int tempsPosList = findWordInTab(tab, wordTemps);

    QString cell(tab.at(tempsPosList));
    QStringList cellData(cell.split(QRegExp("\t")));

    QVector<double> tabNumericData(0);
    double numericData(0);

    int nbColumn(cellData.size());
    int nbRow(tab.size() - tempsPosList -2);
    tabNumericData.push_back(nbColumn);
    tabNumericData.push_back(nbRow);

    //creat a tab witch contain all numeric tab data stacked
    for (int k(0); k<=cellData.size()-1; k++)
    {
        for ( int i(tempsPosList+1); i<=tab.size()-1; i++)
        {
            cell = tab.at(i);
            cellData = cell.split(QRegExp("\t"));
            numericData = cellData[k].toDouble();
            tabNumericData.push_back(numericData);
        }
    }

    return tabNumericData;
}
