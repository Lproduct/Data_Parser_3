#ifndef FENENFANTTAB_H
#define FENENFANTTAB_H

#include <QtWidgets>

class FenEnfantTab : public QTableWidget
{
    Q_OBJECT

public:
    FenEnfantTab();
    bool loadTabData(const QStringList &infoFile, const QStringList &header, const QVector<QVector<double> > &tab);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

signals:
    closedSignal();

private:
    void setInfo(const QStringList &info);
    void setTabParameter(const QStringList &header, const QVector<QVector<double> > &tab);
    void setTab(const QVector<QVector<double> > &tab);

    QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fileName);

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

    QString curFile;
};

#endif // FENENFANTTAB_H
