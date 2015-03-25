#ifndef FENENFANTGRAPH_H
#define FENENFANTGRAPH_H

#include <QWidget>
#include <qcustomplot.h>

namespace Ui {
class FenEnfantGraph;
}

class FenEnfantGraph : public QWidget
{
    Q_OBJECT

public:
    explicit FenEnfantGraph(QWidget *parent = 0);
    ~FenEnfantGraph();

    void LoadTabData(const QVector<double> &tab, const QString &fileName);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

private:
    void setUpCurve();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    Ui::FenEnfantGraph *ui;
    QString demoName;
    QString curFile;
};

#endif // FENENFANTGRAPH_H
