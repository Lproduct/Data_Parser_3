#include "FenEnfantGraph.h"
#include "ui_FenEnfantGraph.h"

FenEnfantGraph::FenEnfantGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FenEnfantGraph)
{
    ui->setupUi(this);
    setMinimumHeight(550);
    setMinimumWidth(600);
    setUpCurve();//ui->customPlot

    setWindowTitle("QCustomPlot: "+demoName);
    ui->customPlot->replot();
}

FenEnfantGraph::~FenEnfantGraph()
{
    delete ui;
}

void FenEnfantGraph::setUpCurve()//QCustomPlot *customPlot
{
    demoName = "Data";
    // generate some data:

}

void FenEnfantGraph::LoadTabData(const QVector<double> &tab)
{
    /*QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i];  // let's plot a quadratic function
    }*/
    int nbColumn(tab.at(0));
    int nbRow(tab.at(1));

    QVector<double> x(nbRow+1), y1(nbRow+1), y2(nbRow+1), y3(nbRow+1), y4(nbRow+1);

    for (int i(0); i<=nbRow; i++)
    {
        x[i] = tab[i+2];
    }

    for (int k=0; k<=nbRow; k++)
    {
        y1[k] = tab[k+2+nbRow+1];
    }

    for (int k=0; k<=nbRow; k++)
    {
        y2[k] = tab[k+2+nbRow+1+nbRow+1];
    }

    for (int k=0; k<=nbRow; k++)
    {
        y3[k] = tab[k+2+nbRow+1+nbRow+1+nbRow+1];
    }

    for (int k=0; k<=nbRow; k++)
    {
        y4[k] = tab[k+2+nbRow+1+nbRow+1+nbRow+1+nbRow+1];
    }

    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y1);

    /*ui->customPlot->graph(1)->setData(x, y2);
    ui->customPlot->graph(2)->setData(x, y3);
    ui->customPlot->graph(3)->setData(x, y4);*/

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->customPlot->xAxis->setRange(0, 1500);
    ui->customPlot->yAxis->setRange(0, 15000);
}
