#include "FenEnfantGraph.h"
#include "ui_FenEnfantGraph.h"

fenEnfantGraph::fenEnfantGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fenEnfantGraph)
{
    ui->setupUi(this);
}

fenEnfantGraph::~fenEnfantGraph()
{
    delete ui;
}
