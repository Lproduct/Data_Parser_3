#include "FenEnfantGraph.h"
#include "ui_FenEnfantGraph.h"

FenEnfantGraph::FenEnfantGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FenEnfantGraph)
{
    ui->setupUi(this);
    setMinimumHeight(550);
    setMinimumWidth(600);
    setUpCurve(ui->customPlot);

    setWindowTitle("QCustomPlot: "+demoName);
    ui->customPlot->replot();
}

FenEnfantGraph::~FenEnfantGraph()
{
    delete ui;
}

void FenEnfantGraph::setUpCurve(QCustomPlot *customPlot)
{
    demoName = "Quadratic Demo";
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(-1, 1);
    customPlot->yAxis->setRange(0, 1);
}
