#include "FenEnfantGraph.h"
#include "ui_FenEnfantGraph.h"


FenEnfantGraph::FenEnfantGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FenEnfantGraph)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);

    ui->customPlot->replot();
}

FenEnfantGraph::~FenEnfantGraph()
{
    delete ui;
}

bool FenEnfantGraph::LoadTabData(const QVector<double> &tab, const QStringList &fileInfo)
{
    setCurrentFile(fileInfo.at(0));

    int nbColumn(tab.at(0));
    int nbRow(tab.at(1));

    QVector<double> x(nbRow+1), y1(nbRow+1), y2(nbRow+1), y3(nbRow+1), y4(nbRow+1), y5(nbRow+1), y6(nbRow+1), y7(nbRow+1), y8(nbRow+1), y9(nbRow+1);

    switch(nbColumn)
    {
    case 1:
        break;

    case 2:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);
        break;

    case 3:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);
        break;

    case 4:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
            y3[i] = tab[i+2+3*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(2)->setData(x, y3);
        break;

    case 5:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
            y3[i] = tab[i+2+3*(nbRow+1)];
            y4[i] = tab[i+2+4*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(2)->setData(x, y3);

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::green));
        ui->customPlot->graph(3)->setData(x, y4);
        break;

    case 6:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
            y3[i] = tab[i+2+3*(nbRow+1)];
            y4[i] = tab[i+2+4*(nbRow+1)];
            y5[i] = tab[i+2+5*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(2)->setData(x, y3);

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::green));
        ui->customPlot->graph(3)->setData(x, y4);

        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::black));
        ui->customPlot->graph(4)->setData(x, y5);
        break;

    case 7:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
            y3[i] = tab[i+2+3*(nbRow+1)];
            y4[i] = tab[i+2+4*(nbRow+1)];
            y5[i] = tab[i+2+5*(nbRow+1)];
            y6[i] = tab[i+2+6*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(2)->setData(x, y3);

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::green));
        ui->customPlot->graph(3)->setData(x, y4);

        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::black));
        ui->customPlot->graph(4)->setData(x, y5);

        ui->customPlot->addGraph();
        ui->customPlot->graph(5)->setPen(QPen(Qt::gray));
        ui->customPlot->graph(5)->setData(x, y6);
        break;

    case 8:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
            y3[i] = tab[i+2+3*(nbRow+1)];
            y4[i] = tab[i+2+4*(nbRow+1)];
            y5[i] = tab[i+2+5*(nbRow+1)];
            y6[i] = tab[i+2+6*(nbRow+1)];
            y7[i] = tab[i+2+7*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(2)->setData(x, y3);

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::green));
        ui->customPlot->graph(3)->setData(x, y4);

        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::black));
        ui->customPlot->graph(4)->setData(x, y5);

        ui->customPlot->addGraph();
        ui->customPlot->graph(5)->setPen(QPen(Qt::gray));
        ui->customPlot->graph(5)->setData(x, y6);

        ui->customPlot->addGraph();
        ui->customPlot->graph(6)->setPen(QPen(Qt::cyan));
        ui->customPlot->graph(6)->setData(x, y7);
        break;

    case 9:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
            y3[i] = tab[i+2+3*(nbRow+1)];
            y4[i] = tab[i+2+4*(nbRow+1)];
            y5[i] = tab[i+2+5*(nbRow+1)];
            y6[i] = tab[i+2+6*(nbRow+1)];
            y7[i] = tab[i+2+7*(nbRow+1)];
            y8[i] = tab[i+2+8*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(2)->setData(x, y3);

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::green));
        ui->customPlot->graph(3)->setData(x, y4);

        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::black));
        ui->customPlot->graph(4)->setData(x, y5);

        ui->customPlot->addGraph();
        ui->customPlot->graph(5)->setPen(QPen(Qt::gray));
        ui->customPlot->graph(5)->setData(x, y6);

        ui->customPlot->addGraph();
        ui->customPlot->graph(6)->setPen(QPen(Qt::cyan));
        ui->customPlot->graph(6)->setData(x, y7);

        ui->customPlot->addGraph();
        ui->customPlot->graph(7)->setPen(QPen(Qt::magenta));
        ui->customPlot->graph(7)->setData(x, y8);
        break;

    case 10:
        for (int i(0); i<=nbRow; i++)
        {
            x[i] = tab[i+2];
            y1[i] = tab[i+2+nbRow+1];
            y2[i] = tab[i+2+2*(nbRow+1)];
            y3[i] = tab[i+2+3*(nbRow+1)];
            y4[i] = tab[i+2+4*(nbRow+1)];
            y5[i] = tab[i+2+5*(nbRow+1)];
            y6[i] = tab[i+2+6*(nbRow+1)];
            y7[i] = tab[i+2+7*(nbRow+1)];
            y8[i] = tab[i+2+8*(nbRow+1)];
            y9[i] = tab[i+2+9*(nbRow+1)];
        }

        // create graph and assign data to it:
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setData(x, y1);

        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->graph(1)->setData(x, y2);

        ui->customPlot->addGraph();
        ui->customPlot->graph(2)->setPen(QPen(Qt::yellow));
        ui->customPlot->graph(2)->setData(x, y3);

        ui->customPlot->addGraph();
        ui->customPlot->graph(3)->setPen(QPen(Qt::green));
        ui->customPlot->graph(3)->setData(x, y4);

        ui->customPlot->addGraph();
        ui->customPlot->graph(4)->setPen(QPen(Qt::black));
        ui->customPlot->graph(4)->setData(x, y5);

        ui->customPlot->addGraph();
        ui->customPlot->graph(5)->setPen(QPen(Qt::gray));
        ui->customPlot->graph(5)->setData(x, y6);

        ui->customPlot->addGraph();
        ui->customPlot->graph(6)->setPen(QPen(Qt::cyan));
        ui->customPlot->graph(6)->setData(x, y7);

        ui->customPlot->addGraph();
        ui->customPlot->graph(7)->setPen(QPen(Qt::magenta));
        ui->customPlot->graph(7)->setData(x, y8);

        ui->customPlot->addGraph();
        ui->customPlot->graph(8)->setPen(QPen(Qt::magenta));
        ui->customPlot->graph(8)->setData(x, y9);
        break;

    }

    // give the axes some labels:
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");

    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->customPlot->graph(0)->rescaleAxes();

    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    //ui->customPlot->graph(1)->rescaleAxes(true);

    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    return true;
}

void FenEnfantGraph::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    //isUntitled = false;
    //document()->setModified(false);
    //setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString FenEnfantGraph::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

QString FenEnfantGraph::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

