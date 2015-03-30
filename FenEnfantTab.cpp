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

bool FenEnfantTab::loadTabData(const QStringList &info,const QStringList &header, const QVector<QVector<double> > &tab)
{
    setInfo(info);

    setTabParameter(header,tab);

    setTab(tab);

    return true;
}

void FenEnfantTab::setInfo(const QStringList &info)
{
    setCurrentFile(info.at(0));
    fichier->setText(info.at(1));
    date->setText(info.at(2));
    heure->setText(info.at(3));
    nomOperateur->setText(info.at(4));
    nomSite->setText(info.at(5));
    nomEssai->setText(info.at(6));
    commentaires->setText(info.at(7));
}

void FenEnfantTab::setTabParameter(const QStringList &header, const QVector<QVector<double> > &tab)
{
    QVector<double> subtab(tab.at(0));
    table->setColumnCount(tab.size());
    table->setRowCount(subtab.size());

    table->setHorizontalHeaderLabels(header);
}

void FenEnfantTab::setTab(const QVector<QVector<double> > &tab)
{
    QVector<double> subTab(tab.at(0));
    int nbColumn(tab.size());
    long int nbRow(subTab.size());

    for(int i(0); i<=nbColumn-1; i++)
    {
        for(long int k(0); k<=nbRow-1; k++)
        {
            table->setItem(k, i, new QTableWidgetItem(QString::number(subTab.at(k))));
        }
    }
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

void FenEnfantTab::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit closedSignal();
}


