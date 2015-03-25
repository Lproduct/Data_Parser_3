#ifndef FENENFANTTAB_H
#define FENENFANTTAB_H

#include <QtWidgets>


class FenEnfantTab : public QTableWidget
{
    Q_OBJECT

public:
    FenEnfantTab();
    bool loadFileTab(const QString &fileName);
    QVector<double> loadNumericData(const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

signals:
    closedSignal();

private:
    int findWordInTab(const QStringList& tab, const QString& word);
    void generateInfoWidget(const QStringList& tab);
    void generateTabWidget(const QStringList& tab);
    void genererWidget(const QStringList& tab);
    void parsingDataIntoWidget(const QString dataTxt);
    QString fileTxtInfo(const QString& info);
    QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fileName);
    QVector<double> parsingDataIntoTab(const QString dataTxt);
    void closeEvent(QCloseEvent *e);
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
