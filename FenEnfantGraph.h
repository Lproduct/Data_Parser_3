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

private:
    void setUpCurve(QCustomPlot *customPlot);
    Ui::FenEnfantGraph *ui;
    QString demoName;
};

#endif // FENENFANTGRAPH_H
