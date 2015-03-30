#include "FenEnfantGraph.h"
#include "ui_FenEnfantGraph.h"
#include "time.h"
#include <algorithm>


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

bool FenEnfantGraph::LoadTabData(const QStringList &fileInfo, const QStringList &header,const QVector<QVector<double> > &tab )
{
    setCurrentFile(fileInfo.at(0));    

    setGraphParameter();
    setGraph(header, tab);
    averageCurve(1, 30, tab);
    middleCurve(1, 30, tab);
    defineAxis(header);


    return true;
}

void FenEnfantGraph::setGraphParameter()
{
    // configure right and top axis to show ticks but no labels:
    // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
    //ui->customPlot->graph(1)->rescaleAxes(true);

    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //Set legend on the graph
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));
}

void FenEnfantGraph::setGraph(const QStringList &header, const QVector<QVector<double> > &tab)
{
    for(int i(0); i<tab.size()-1; i++)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(i)->setPen(QPen(randomColor("normal")));
        ui->customPlot->graph(i)->setName(header.at(i+1));
        ui->customPlot->graph(i)->setData(tab.at(0), tab.at(i+1));
    }
}

void FenEnfantGraph::defineAxis(const QStringList &header)
{
    // give the axes some labels:
     ui->customPlot->xAxis->setLabel(header.at(0));
     ui->customPlot->yAxis->setLabel("Comptage");

     // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
     ui->customPlot->graph(0)->rescaleAxes();
}

QColor FenEnfantGraph::randomColor(const QString &colorType)
{
    int modulo(0);
    if (colorType == "normal")
    {
        modulo = 8;
    }
    if (colorType == "all")
    {
        modulo = 16;
    }

    int randNum(-1);
    QColor *color =new QColor;
    randNum = rand()%modulo;

    switch(randNum)
    {
    case 0:
        {
            color->setNamedColor("#000000");//black
            break;
        }

    case 1:
        {
            color->setNamedColor("#ff0000");//red
            break;
        }
    case 2:
        {
            color->setNamedColor("#00ff00");//green
            break;
        }
    case 3:
        {
            color->setNamedColor("#0000ff");//blue
            break;
        }
    case 4:
        {
            color->setNamedColor("#00ffff");//cyan
            break;
        }
    case 5:
        {
            color->setNamedColor("#ff00ff");//magenta
            break;
        }
    case 6:
        {
            color->setNamedColor("#ffff00");//yellow
            break;
        }
    case 7:
        {
            color->setNamedColor("#a0a0a4");//grey
            break;
        }
    case 8:
        {
            color->setNamedColor("#800000");//darkRed
            break;
        }
    case 9:
        {
            color->setNamedColor("#008000");//darkGreen
            break;
        }
    case 10:
        {
            color->setNamedColor("#000080");//darkBlue
            break;
        }
    case 11:
        {
            color->setNamedColor("#008080");//darkCyan
            break;
        }
    case 12:
        {
            color->setNamedColor("#800080");//darkMagenta
            break;
        }
    case 13:
        {
            color->setNamedColor("#808000");//darkYellow
            break;
        }
    case 14:
        {
            color->setNamedColor("#808000");//darkGray
            break;
        }
    case 15:
        {
            color->setNamedColor("#808000");//lightGray
            break;
        }

    }

    return *color;
}

void FenEnfantGraph::averageCurve(const int &nbTab, const int &sampleTime,const QVector<QVector<double> > &tab)
{
    //Create a average curve
    QVector<double> subTab(tab.at(0));

    //int nbColumn(tab.size());
    int nbRow(subTab.size());

    int reste(nbRow%sampleTime);
    int nbLoop(0);
    if (reste == 0)
    {
        nbLoop =nbRow/sampleTime;
    }
    else
    {
        nbLoop = (nbRow-reste)/sampleTime;
    }

    QVector<double> axisTab;
    double sampleTimeModif = (double) sampleTime;
    for(int i(0); i<=nbLoop-1; i++)
    {
        axisTab.push_back(sampleTimeModif/2+i*sampleTimeModif);
    }

    QVector<double> ordTab;
    QVector<double> subtab2(tab.at(nbTab));
    for(int i(0); i<=nbLoop-1; i++)
    {
        double average(0);
        for(int k(0+i*(sampleTime)); k<=sampleTime-1+i*(sampleTime); k++)
        {
            average += subtab2.at(k);
        }
        ordTab.push_back(average/sampleTime);
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(QPen(randomColor("normal")));
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setName("average");
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setData(axisTab, ordTab);
}

void FenEnfantGraph::middleCurve(const int &nbTab, const int &sampleTime,const QVector<QVector<double> > &tab)
{
    //Create a value middle curve
    int sampleTimeModif;
    if (sampleTime%2 == 0)
    {
        sampleTimeModif = sampleTime +1;
    }
    else
    {
        sampleTimeModif = sampleTime;
    }

    QVector<double> subTab(tab.at(0));

    //int nbColumn(tab.size());
    int nbRow(subTab.size());

    int reste(nbRow%sampleTimeModif);
    int nbLoop(0);
    if (reste == 0)
    {
        nbLoop =nbRow/sampleTimeModif;
    }
    else
    {
        nbLoop = (nbRow-reste)/sampleTimeModif;
    }

    QVector<double> axisTab;
    double sampleTimeModifDouble = (double) sampleTimeModif;
    for(int i(0); i<=nbLoop-1; i++)
    {
        axisTab.push_back(sampleTimeModifDouble/2+i*sampleTimeModifDouble);
    }

    QVector<double> ordTab;
    QVector<double> subtab2(tab.at(nbTab));
    for(int i(0); i<=nbLoop-1; i++)
    {
        QVector<double> middleTab(0);

        for(int k(0+i*(sampleTimeModif)); k<=sampleTimeModif-1+i*(sampleTimeModif); k++)
        {
            middleTab.push_back(subtab2.at(k));
        }
        std::sort(middleTab.begin(), middleTab.end());
        ordTab.push_back(middleTab.at(sampleTimeModif/2));
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(QPen(randomColor("normal")));
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setName("middle");
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setData(axisTab, ordTab);

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

