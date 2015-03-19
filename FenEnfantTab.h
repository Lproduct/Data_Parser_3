#ifndef FENENFANTTAB_H
#define FENENFANTTAB_H

#include <QtWidgets>


class FenEnfantTab : public QTableWidget
{
    Q_OBJECT

public:
    FenEnfantTab();
    bool loadFileTab(const QString &fileName);

private:
    int findWordInTab(const QStringList& tab, const QString& word);
    void generateInfoWidget(const QStringList& tab);
    void generateTabWidget(const QStringList& tab);
    void genererWidget(const QStringList& tab, const QFileInfo fi);
    void parsingDataIntoWidget(const QString dataTxt, const QFileInfo fi);
    QString fileTxtInfo(const QString& info);

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

};

#endif // FENENFANTTAB_H
