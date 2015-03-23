#ifndef FENENFANTTAB_H
#define FENENFANTTAB_H

#include <QtWidgets>


class FenEnfantTab : public QTableWidget
{
    Q_OBJECT

public:
    FenEnfantTab();
    bool loadFileTab(const QString &fileName);
    QString userFriendlyCurrentFile();

private:
    int findWordInTab(const QStringList& tab, const QString& word);
    void generateInfoWidget(const QStringList& tab);
    void generateTabWidget(const QStringList& tab);
    void genererWidget(const QStringList& tab);
    void parsingDataIntoWidget(const QString dataTxt);
    QString fileTxtInfo(const QString& info);
    QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fileName);

    QTableWidget *table;
    QTableWidgetItem *tableItem;
    QLineEdit *fichier;
    QLineEdit *date;
    QLineEdit *heure;
    QLineEdit *nomOperateur;
    QLineEdit *nomSite;
    QLineEdit *nomEssai;
    QLineEdit *commentaires;

    QVBoxLayout *layoutPrincipal;
    QString FileName;
    QString curFile;

};

#endif // FENENFANTTAB_H
