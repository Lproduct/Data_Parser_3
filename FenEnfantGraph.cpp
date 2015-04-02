#include "FenEnfantGraph.h"
#include "ui_FenEnfantGraph.h"
#include "time.h"
#include <algorithm>


FenEnfantGraph::FenEnfantGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FenEnfantGraph)
{
    ui->setupUi(this);

    ui->customPlot->replot();
    dataTimer.start(0);

    ui->curseurEnable->setChecked(true);

    connect(ui->curseurEnable, SIGNAL(stateChanged(int)), this, SLOT(cursorEnable(int)));
    connect(ui->curseur1, SIGNAL(valueChanged(double)), this, SLOT(cursor1(double)) );
    connect(ui->curseur2, SIGNAL(valueChanged(double)), this, SLOT(cursor2(double)) );
    connect(this, SIGNAL(cursor1Update()), ui->customPlot, SLOT(replot()) );
    connect(this, SIGNAL(cursor2Update()), ui->customPlot, SLOT(replot()) );
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(cursorHeightScroll(QWheelEvent*)));
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(cursorHeightMouved(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(cursorHeightPressed(QMouseEvent*)));

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(curveDisplay(int)));
}

FenEnfantGraph::~FenEnfantGraph()
{
    delete ui;
}

bool FenEnfantGraph::LoadTabData(const QStringList &fileInfo, const QStringList &header,const QVector<QVector<double> > &tab )
{
    setCurrentFile(fileInfo.at(0));    
    setInfoData(fileInfo);

    setGraphParameter();
    setGraph(header, tab);

    averageCurve(1, 9, tab);
    middleCurve(1, 9, tab);
    setCursorCurveV1(0, ui->customPlot->graphCount(), ui->customPlot->yAxis->range());
    setCursorCurveV2(0, ui->customPlot->graphCount(), ui->customPlot->yAxis->range());

    defineAxis(header);

    return true;
}

void FenEnfantGraph::setInfoData(const QStringList &fileInfo)
{
    ui->title->setText(fileInfo.at(1));
    ui->date->setText(fileInfo.at(2));
    ui->heure->setText(fileInfo.at(3));
    ui->nomOperateur->setText(fileInfo.at(4));
    ui->nomSite->setText(fileInfo.at(5));
    ui->nomEssai->setText(fileInfo.at(6));
    ui->commentaire->setText(fileInfo.at(7));
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
        setTabCurve(header.at(i+1));
        indexGraph[header.at(i+1)] = i;
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
    setTabCurve("average");
    indexGraph["average"] = (ui->customPlot->graphCount()-1);
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
    setTabCurve("middle");
    indexGraph["middle"] = (ui->customPlot->graphCount()-1);
}

void FenEnfantGraph::setCursorCurveV1(const double &posCursor, const int &nbGraph, const QCPRange &cursorHeight)
{
    QVector<double> absTab;
    for(int i(0); i <= 1; i++)
    {
        absTab.push_back(posCursor);
    }

    double cursorHeightMin(cursorHeight.lower);
    double cursorHeightMax(cursorHeight.upper);
    QVector<double> ordTab;
    ordTab.push_back(cursorHeightMin);
    ordTab.push_back(cursorHeightMax);


    QString name("cursor 1");
    QPen pen;
    pen.setColor(QColor(255,170,100));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);

    if(indexGraph[name] == 0)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(nbGraph)->setPen(pen);
        ui->customPlot->graph(nbGraph)->setName(name);
        ui->customPlot->graph(nbGraph)->setData(absTab, ordTab);
        setTabCurve(name);
        indexGraph[name] = (nbGraph);
    }
    else
    {
        ui->customPlot->graph(nbGraph)->setData(absTab, ordTab);
    }
}

void FenEnfantGraph::setCursorCurveV2(const double &posCursor, const int &nbGraph, const QCPRange &cursorHeight)
{
    QVector<double> absTab;
    for(int i(0); i <= 1; i++)
    {
        absTab.push_back(posCursor);
    }

    double cursorHeightMin(cursorHeight.lower);
    double cursorHeightMax(cursorHeight.upper);
    QVector<double> ordTab;
    ordTab.push_back(cursorHeightMin);
    ordTab.push_back(cursorHeightMax);


    QString name("cursor 2");
    QPen pen;
    pen.setColor(QColor(255,170,100));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);

    if(indexGraph[name] == 0)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(nbGraph)->setPen(pen);
        ui->customPlot->graph(nbGraph)->setName(name);
        ui->customPlot->graph(nbGraph)->setData(absTab, ordTab);
        setTabCurve(name);
        indexGraph[name] = (nbGraph);
    }
    else
    {
        ui->customPlot->graph(nbGraph)->setData(absTab, ordTab);
    }
}

void FenEnfantGraph::cursor1(const double &posCursor)
{
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV1(posCursor, indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();
}

void FenEnfantGraph::cursor2(const double &posCursor)
{
    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV2(posCursor, indexGraph["cursor 2"],ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightScroll(QWheelEvent* event )
{
    event->accept();
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV1(ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV2(ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightMouved(QMouseEvent* event )
{
    event->accept();
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV1(ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV2(ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightPressed(QMouseEvent* event )
{
    event->accept();
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV1(ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV2(ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorEnable(const int& state)
{
    if(state == 0)
    {
        ui->curseur1->setEnabled(false);
        ui->curseur2->setEnabled(false);
        ui->customPlot->graph(indexGraph["cursor 1"])->setVisible(false);
        ui->customPlot->graph(indexGraph["cursor 2"])->setVisible(false);
        emit cursor1Update();
        emit cursor2Update();
    }
    if(state == 2)
    {
        ui->curseur1->setEnabled(true);
        ui->curseur2->setEnabled(true);
        ui->customPlot->graph(indexGraph["cursor 1"])->setVisible(true);
        ui->customPlot->graph(indexGraph["cursor 2"])->setVisible(true);
        emit cursor1Update();
        emit cursor2Update();
    }
}

void FenEnfantGraph::setTabCurve(const QString &nameCurve)
{
    //Create a list of curve name that can be checked
    ui->table->setColumnCount(1);
    int nbRow(ui->table->rowCount());
    ui->table->setRowCount(nbRow+1);

    QCheckBox *checkCurve = new QCheckBox;
    checkCurve->setChecked(true);
    checkCurve->setText(nameCurve);
    ui->table->setCellWidget(nbRow, 0, checkCurve);
    connect(checkCurve, SIGNAL(clicked()), signalMapper, SLOT(map()));
    signalMapper->setMapping(checkCurve, nbRow);

}

void FenEnfantGraph::curveDisplay(const int &nbCurve)
{
    bool graphState(ui->customPlot->graph(nbCurve)->visible());

    if (graphState == true)
    {
        ui->customPlot->graph(nbCurve)->setVisible(false);
        ui->customPlot->replot();
    }
    else
    {
        ui->customPlot->graph(nbCurve)->setVisible(true);
        ui->customPlot->replot();
    }
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

