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

    bool LoadTabData(const QVector<double> &tab, const QStringList &fileInfo);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

private:
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    Ui::FenEnfantGraph *ui;
    QString curFile;
};

#endif // FENENFANTGRAPH_H
