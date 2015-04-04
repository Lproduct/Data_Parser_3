#include "FenEnfantGraph.h"
#include "MathFunction.h"
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

    //Cusor handle
    ui->curseurEnable->setChecked(false);
    ui->curseur1->setEnabled(false);
    ui->curseur2->setEnabled(false);

    connect(ui->curseurEnable, SIGNAL(stateChanged(int)), this, SLOT(cursorEnable(int)));
    connect(ui->curseur1, SIGNAL(valueChanged(double)), this, SLOT(cursor1(double)) );
    connect(ui->curseur2, SIGNAL(valueChanged(double)), this, SLOT(cursor2(double)) );
    connect(this, SIGNAL(cursor1Update()), ui->customPlot, SLOT(replot()) );
    connect(this, SIGNAL(cursor2Update()), ui->customPlot, SLOT(replot()) );
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(cursorHeightScroll(QWheelEvent*)));
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(cursorHeightMouved(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(cursorHeightPressed(QMouseEvent*)));

    //Tab curve list
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(curveDisplay(int)));

    //Zoom manager
    connect(ui->checkBoxZoomV, SIGNAL(stateChanged(int)), this, SLOT(zoom()));
    connect(ui->checkBoxZoomH, SIGNAL(stateChanged(int)), this, SLOT(zoom()));

    //Create Curve
    connect(ui->pushButtoncreateCurve, SIGNAL(clicked()), this,SLOT(createCurve()));
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

    mathMethod = new MathFunction(tab);

    initCursor();

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
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );
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
    createCurveIntialisation();
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

//Cursor
    //Function
void FenEnfantGraph::initCursor()
{
    setCursorCurveV(1, 0, ui->customPlot->graphCount(), ui->customPlot->yAxis->range());
    setCursorCurveV(2, 0, ui->customPlot->graphCount(), ui->customPlot->yAxis->range());
}

void FenEnfantGraph::setCursorCurveV(const int cursorId, const double &posCursor, const int &nbGraph, const QCPRange &cursorHeight)
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


    QString name(tr("cursor %1").arg(cursorId));
    QPen pen;
    pen.setColor(QColor(255,170,100));
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);

    int occur(0);
    for(std::map<QString, int>::iterator it(indexGraph.begin()); it != indexGraph.end(); it++)
    {
        if(it->first == name)
        {
            occur++;
        }
    }

    if(occur == 0)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(nbGraph)->setPen(pen);
        ui->customPlot->graph(nbGraph)->setName(name);
        ui->customPlot->graph(nbGraph)->setData(absTab, ordTab);
        ui->customPlot->graph(nbGraph)->setVisible(false);
        ui->customPlot->graph(nbGraph)->removeFromLegend();

        //setTabCurve(name);
        indexGraph[name] = (nbGraph);
    }
    else
    {
        ui->customPlot->graph(nbGraph)->setData(absTab, ordTab);
    }
}

void FenEnfantGraph::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Plus)
    {
        if(ui->customPlot->graph(indexGraph["cursor 1"])->selected() == true)
        {
           ui->curseur1->setValue(ui->curseur1->value()+1);
        }
        else
        {
            event->ignore();
        }

        if(ui->customPlot->graph(indexGraph["cursor 2"])->selected() == true)
        {
           ui->curseur2->setValue(ui->curseur2->value()+1);
        }
        else
        {
            event->ignore();
        }
    }

    if(event->key() == Qt::Key_Minus)
    {
        if(ui->customPlot->graph(indexGraph["cursor 1"])->selected() == true)
        {
           ui->curseur1->setValue(ui->curseur1->value()-1);
        }
        else
        {
            event->ignore();
        }

        if(ui->customPlot->graph(indexGraph["cursor 2"])->selected() == true)
        {
           ui->curseur2->setValue(ui->curseur2->value()-1);
        }
        else
        {
            event->ignore();
        }
    }

    else
    {
        event->ignore();
    }
}
    //SLOT
void FenEnfantGraph::cursorEnable(const int& state)
{
    if(state == 0)
    {
        ui->curseur1->setEnabled(false);
        ui->curseur2->setEnabled(false);
        ui->customPlot->graph(indexGraph["cursor 1"])->setVisible(false);
        ui->customPlot->graph(indexGraph["cursor 2"])->setVisible(false);
        ui->customPlot->graph(indexGraph["cursor 1"])->removeFromLegend();
        ui->customPlot->graph(indexGraph["cursor 2"])->removeFromLegend();
        emit cursor1Update();
        emit cursor2Update();
    }
    if(state == 2)
    {
        ui->curseur1->setEnabled(true);
        ui->curseur2->setEnabled(true);
        ui->customPlot->graph(indexGraph["cursor 1"])->setVisible(true);
        ui->customPlot->graph(indexGraph["cursor 2"])->setVisible(true);
        ui->customPlot->graph(indexGraph["cursor 1"])->addToLegend();
        ui->customPlot->graph(indexGraph["cursor 2"])->addToLegend();
        emit cursor1Update();
        emit cursor2Update();
    }
}

void FenEnfantGraph::cursor1(const double &posCursor)
{
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV(1, posCursor, indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();
}

void FenEnfantGraph::cursor2(const double &posCursor)
{
    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV(2, posCursor, indexGraph["cursor 2"],ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightScroll(QWheelEvent* event )
{
    event->accept();
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV(1, ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV(2, ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightMouved(QMouseEvent* event )
{
    event->accept();
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV(1, ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV(1, ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightPressed(QMouseEvent* event )
{
    event->accept();
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV(1, ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV(1, ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}
//Cursor end

//Tab Curve display
    //function
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
    //SLOT
void FenEnfantGraph::curveDisplay(const int &nbCurve)
{
    bool graphState(ui->customPlot->graph(nbCurve)->visible());

    if (graphState == true)
    {
        ui->customPlot->graph(nbCurve)->setVisible(false);
        ui->customPlot->graph(nbCurve)->removeFromLegend();
        ui->customPlot->replot();
    }
    else
    {
        ui->customPlot->graph(nbCurve)->setVisible(true);
        ui->customPlot->graph(nbCurve)->addToLegend();
        ui->customPlot->replot();
    }
}
//Tab Curve display end

//Zoom manager
    //SLOT
void FenEnfantGraph::zoom()
{
    int stateZoomV(ui->checkBoxZoomV->checkState());
    int stateZoomH(ui->checkBoxZoomH->checkState());

    if(stateZoomV == 2 && stateZoomH == 2)
    {
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );
        //ui->customPlot->axisRect(QCP::iRangeZoom);
        ui->customPlot->axisRect()->setRangeZoomFactor(1.25, 1.25);
    }

    else if(stateZoomV == 0 && stateZoomH == 2)
    {
        //ui->customPlot->setInteraction(QCP::iRangeDrag | QCP::iSelectPlottables);
        ui->customPlot->axisRect()->setRangeZoomFactor(1.25, 0);
    }

    else if(stateZoomV == 2 && stateZoomH == 0)
    {
        //ui->customPlot->setInteraction(QCP::iRangeDrag | QCP::iSelectPlottables);
        ui->customPlot->axisRect()->setRangeZoomFactor(0, 1.25);
    }

    else if(stateZoomV == 0 && stateZoomH == 0)
    {
        //ui->customPlot->setInteraction(QCP::iRangeDrag | QCP::iSelectPlottables);
        ui->customPlot->axisRect()->setRangeZoomFactor(0, 0);
    }

}
//Zoom manager end

//Math curve display
    //function
void FenEnfantGraph::createCurveIntialisation()
{
    ui->comboBoxCurveType->setEnabled(true);
    ui->comboBoxCurveType->setEditable(false);
    ui->comboBoxCurveType->clear();
    ui->comboBoxCurveType->addItem("Moyenne");
    ui->comboBoxCurveType->addItem("Filtre mileu");

    ui->ComboBoxCurveName->setEnabled(true);
    for(std::map<QString,int>::iterator it(indexGraph.begin()); it!= indexGraph.end(); it++)
    {
        QString nameCurve(it->first);
        if (nameCurve == "cursor 1" || nameCurve == "cursor 2")
        {

        }
        else
        {
          ui->ComboBoxCurveName->addItem(nameCurve);
        }
    }

    ui->spinBoxSampleTime->setEnabled(true);

    ui->pushButtoncreateCurve->setEnabled(true);
}

QString FenEnfantGraph::curveName(const QVector<double> &tabId)
{
    double opId(tabId.at(0));
    QString name;

    if(opId == 0)
    {
        name = tr("middle_%1_ST:%2").arg(ui->ComboBoxCurveName->currentText())
                                 .arg(ui->spinBoxSampleTime->value());
    }

    if(opId == 1)
    {
        name = tr("average_%1_ST:%2").arg(ui->ComboBoxCurveName->currentText())
                            .arg(ui->spinBoxSampleTime->value());
    }

    return name;
}
    //SLOT
void FenEnfantGraph::createCurve()
{
    QString curveType(ui->comboBoxCurveType->currentText());
    QString curveselected(ui->ComboBoxCurveName->currentText());
    double sampleTime(ui->spinBoxSampleTime->value());

    if(curveType == "Moyenne")
    {
       displayMathFunctionCurve(mathMethod->averageValueCurve(indexGraph[curveselected]+1, sampleTime));
    }

    else if(curveType == "Filtre mileu")
    {
       displayMathFunctionCurve(mathMethod->middleValueCurveFilter(indexGraph[curveselected]+1, sampleTime));
    }
}

void FenEnfantGraph::displayMathFunctionCurve(const QVector<QVector<double> > &tab)
{
    //Kill cursor
    ui->customPlot->removeGraph(indexGraph["cursor 2"]);
    ui->customPlot->removeGraph(indexGraph["cursor 1"]);
    for(std::map<QString, int>::iterator it(indexGraph.begin()); it != indexGraph.end(); it++)
    {
        if (it->first == "cursor 1")
        {
              indexGraph.erase(it);
              break;
        }

    }

    for(std::map<QString, int>::iterator it(indexGraph.begin()); it != indexGraph.end(); it++)
    {
        if (it->first == "cursor 2")
        {
              indexGraph.erase(it);
              break;
        }
    }
    //insert curve
    ui->customPlot->addGraph();
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(QPen(randomColor("normal")));
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setName(curveName(tab.at(2)));
    QVector<double> test(tab.at(0));
    QVector<double> test2(tab.at(1));
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setData(tab.at(0), tab.at(1));

    setTabCurve(curveName(tab.at(2)));
    indexGraph[curveName(tab.at(2))] = ui->customPlot->graphCount()-1;

    //set Cursor
    setCursorCurveV(1, ui->curseur1->value(), ui->customPlot->graphCount(), ui->customPlot->yAxis->range());
    setCursorCurveV(2, ui->curseur2->value(), ui->customPlot->graphCount(), ui->customPlot->yAxis->range());

    if(ui->curseurEnable->checkState() == 2)
    {
        ui->customPlot->graph(indexGraph["cursor 1"])->setVisible(true);
        ui->customPlot->graph(indexGraph["cursor 2"])->setVisible(true);
        ui->customPlot->graph(indexGraph["cursor 1"])->addToLegend();
        ui->customPlot->graph(indexGraph["cursor 2"])->addToLegend();
    }
    else
    {
    }

    ui->customPlot->replot();
}
//Math curve display end

//FenPrincipal Widget manager
    //Function
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
//FenPrincipal Widget manager end
