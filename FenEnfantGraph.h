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

    bool LoadTabData(const QStringList &fileInfo, const QStringList &header, const QVector<QVector<double> > &tab);
    QString userFriendlyCurrentFile();
    void dataAverage(const QVector<double> &tab);
    QString currentFile() { return curFile; }

private:
    void setCurrentFile(const QString &fileName);
    QColor randomColor();
    QString strippedName(const QString &fullFileName);
    Ui::FenEnfantGraph *ui;
    QString curFile;
};

#endif // FENENFANTGRAPH_H
