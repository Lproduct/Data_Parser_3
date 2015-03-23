#ifndef FENENFANTGRAPH_H
#define FENENFANTGRAPH_H

#include <QWidget>

namespace Ui {
class fenEnfantGraph;
}

class fenEnfantGraph : public QWidget
{
    Q_OBJECT

public:
    explicit fenEnfantGraph(QWidget *parent = 0);
    ~fenEnfantGraph();

private:
    Ui::fenEnfantGraph *ui;
};

#endif // FENENFANTGRAPH_H
