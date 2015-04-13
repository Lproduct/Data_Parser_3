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

    // setup policy and connect slot for context menu popup:
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    // connect some interaction slots:
    connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(ui->customPlot, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)), this, SLOT(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)));

    // connect slot that ties some axis selections together:
    //connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));

    //Connect export graph as PNG, JPEG, PDF to interface
    ui->pushButtonLinkPng->setCheckable(true);
    ui->pushButtonLinkPng->setChecked(true);
    ui->pushButtonLinkJpeg->setCheckable(true);
    ui->pushButtonLinkJpeg->setChecked(true);

    connect(ui->spinBoxXSizePng, SIGNAL(valueChanged(int)), this, SLOT(linkPngValueY(int)));
    connect(ui->spinBoxYSizePng, SIGNAL(valueChanged(int)), this, SLOT(linkPngValueX(int)));

    connect(ui->spinBoxXSizeJpeg, SIGNAL(valueChanged(int)), this, SLOT(linkJpegValueY(int)));
    connect(ui->spinBoxYSizeJpeg, SIGNAL(valueChanged(int)), this, SLOT(linkJpegValueX(int)));

    connect(ui->pushButtonSavAsPng, SIGNAL(clicked()), this, SLOT(exportGraphAsPng()));
    connect(ui->pushButtonSaveAsPdf, SIGNAL(clicked()), this, SLOT(exportGraphAsPdf()));
    connect(ui->pushButtonSaveAsJpeg, SIGNAL(clicked()), this, SLOT(exportGraphAsJpeg()));

    //Connect time management
    connect(ui->checkBoxTimeAbsis, SIGNAL(clicked()), this, SLOT(setGraphAbsisTime()));
    connect(ui->spinBoxTimeUnit, SIGNAL(valueChanged(int)), this, SLOT(changeAbsisName()));
    connect(ui->comboBoxTimeUnit, SIGNAL(currentTextChanged(QString)), this, SLOT(changeAbsisName()));
}

FenEnfantGraph::~FenEnfantGraph()
{
    delete ui;
}

//FenEnfantGraph parameters
bool FenEnfantGraph::LoadTabData(const QStringList &fileInfo, const QStringList &header,const QVector<QVector<double> > &tab )
{
    setCurrentFile(fileInfo.at(0));    
    setInfoData(fileInfo);
    calculateOffsetTime(fileInfo);

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
    graphTitle = ((QFileInfo(fileInfo.at(0)).fileName()).split(".")).at(0);
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

    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables );
    //Set legend on the graph
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,150)));

    //Set title
    ui->customPlot->plotLayout()->insertRow(0);
    //graphTitle = "Untitle";
    ui->customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui->customPlot, graphTitle));
    //Set background default style
    changeColorBGW();
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
     ui->customPlot->xAxis->setLabel(header.at(0) + "(s)");
     m_header = header;
     ui->customPlot->yAxis->setLabel("Comptage");

     // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
     ui->customPlot->graph(0)->rescaleAxes();
}

void FenEnfantGraph::contextMenuRequest(QPoint pos)
{
    QMenu *contextMenu = new QMenu(this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose);

    if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
    {
      contextMenu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
      contextMenu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
      contextMenu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
      contextMenu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
      contextMenu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
    }
    else  // general context menu on graphs requested
    {
        if (ui->customPlot->selectedGraphs().size() > 0)
        {
          contextMenu->addAction("Change curve color", this, SLOT(changeSelectedGraphColor()));
          contextMenu->addAction("Change curve thickness", this, SLOT(changeSelectedGraphThickness()));
          contextMenu->addAction("Change curve scatter", this, SLOT(changeSelectedGraphScatter()));
        }
        else
        {
            if(blackTheme == false)
            {
                QAction *actionChangeBGB = contextMenu->addAction("Black theme");
                connect(actionChangeBGB, SIGNAL(triggered()), this, SLOT(changeColorBGB()));
            }
            else if(blackTheme == true)
            {
                QAction *actionChangeBGW = contextMenu->addAction("White theme");
                connect(actionChangeBGW, SIGNAL(triggered()), this, SLOT(changeColorBGW()));
            }
        }
    }

    contextMenu->popup(ui->customPlot->mapToGlobal(pos));
}

void FenEnfantGraph::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot->replot();
    }
  }
}

void FenEnfantGraph::changeSelectedGraphColor()
{
    QPen *pen = new QPen(ui->customPlot->selectedGraphs().first()->pen());
    int width(pen->width());

    QColor newColor = QColorDialog::getColor(Qt::black, this);

    QPen newPen;
    newPen.setWidth(width);
    newPen.setColor(newColor);
    ui->customPlot->selectedGraphs().first()->setPen(newPen);
    ui->customPlot->replot();
}

void FenEnfantGraph::changeColorBGB()
{
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));

    ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));

    ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));

    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);

    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridVisible(true);
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->xAxis->grid()->setZeroLinePen(QPen(QColor(140, 140, 140), 1, Qt::SolidLine));
    ui->customPlot->yAxis->grid()->setZeroLinePen(QPen(QColor(140, 140, 140), 1, Qt::SolidLine));

    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QLinearGradient plotGradient;
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->customPlot->setBackground(plotGradient);

    QLinearGradient axisRectGradient;
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->customPlot->axisRect()->setBackground(axisRectGradient);

    ui->customPlot->xAxis->setLabelColor(Qt::white);
    ui->customPlot->yAxis->setLabelColor(Qt::white);

    ui->customPlot->plotLayout()->remove(ui->customPlot->plotLayout()->element(0,0));
    QCPPlotTitle *plotTitle = new QCPPlotTitle(ui->customPlot, graphTitle);
    plotTitle->setTextColor(Qt::white);
    ui->customPlot->plotLayout()->addElement(0, 0, plotTitle);

    ui->customPlot->replot();

    blackTheme = true;
}

void FenEnfantGraph::changeColorBGW()
{
    ui->customPlot->xAxis->setBasePen(QPen(Qt::black, 1));
    ui->customPlot->yAxis->setBasePen(QPen(Qt::black, 1));

    ui->customPlot->xAxis->setTickPen(QPen(Qt::black, 1));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::black, 1));

    ui->customPlot->xAxis->setSubTickPen(QPen(Qt::black, 1));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::black, 1));

    ui->customPlot->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot->yAxis->setTickLabelColor(Qt::black);

    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));

    ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(220, 220, 220), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(220, 220, 220), 1, Qt::DotLine));

    ui->customPlot->xAxis->grid()->setSubGridVisible(true);
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);

    ui->customPlot->xAxis->grid()->setZeroLinePen(QPen(QColor(140, 140, 140), 1, Qt::SolidLine));
    ui->customPlot->yAxis->grid()->setZeroLinePen(QPen(QColor(140, 140, 140), 1, Qt::SolidLine));

    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QLinearGradient plotGradient;
    plotGradient.setColorAt(0, QColor(255, 255, 255));
    plotGradient.setColorAt(1, QColor(255, 255, 255));
    ui->customPlot->setBackground(plotGradient);

    QLinearGradient axisRectGradient;
    axisRectGradient.setColorAt(0, QColor(255, 255, 255));
    axisRectGradient.setColorAt(1, QColor(255, 255, 255));
    ui->customPlot->axisRect()->setBackground(axisRectGradient);

    ui->customPlot->xAxis->setLabelColor(Qt::black);
    ui->customPlot->yAxis->setLabelColor(Qt::black);

    ui->customPlot->plotLayout()->remove(ui->customPlot->plotLayout()->element(0,0));
    QCPPlotTitle *plotTitle = new QCPPlotTitle(ui->customPlot, graphTitle);
    plotTitle->setTextColor(Qt::black);
    ui->customPlot->plotLayout()->addElement(0, 0, plotTitle);

    ui->customPlot->replot();

    blackTheme = false;
}

void FenEnfantGraph::changeSelectedGraphThickness()
{
    QPen *pen = new QPen(ui->customPlot->selectedGraphs().first()->pen());
    QColor color(pen->color());

    int newValue = QInputDialog::getInt(this, "Data Parser", "New curve thickness:", 0, 0, 100);

    QPen newPen;
    newPen.setColor(color);
    newPen.setWidth(newValue);

    ui->customPlot->selectedGraphs().first()->setPen(newPen);
    ui->customPlot->replot();
}

void FenEnfantGraph::changeSelectedGraphScatter()
{
    QStringList item;
    item.push_back("None");
    item.push_back("Cross");
    item.push_back("Circle");
    item.push_back("Disc");
    QString newItem = QInputDialog::getItem(this, "Data Parser", "New curve scatter:",item, 0, false);

    if (newItem == "None")
    {
        ui->customPlot->selectedGraphs().first()->setScatterStyle(QCPScatterStyle::ssNone);
    }
    else if (newItem == "Cross")
    {
        ui->customPlot->selectedGraphs().first()->setScatterStyle(QCPScatterStyle::ssCross);
    }
    else if (newItem == "Circle")
    {
        ui->customPlot->selectedGraphs().first()->setScatterStyle(QCPScatterStyle::ssCircle);
    }
    else if (newItem == "Disc")
    {
        ui->customPlot->selectedGraphs().first()->setScatterStyle(QCPScatterStyle::ssDisc);
    }
    ui->customPlot->replot();
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

void FenEnfantGraph::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "Data Parser", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}

void FenEnfantGraph::titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title)
{
  Q_UNUSED(event)
  // Set the plot title by double clicking on it
  bool ok;
  QString newTitle = QInputDialog::getText(this, "Data Parser", "New plot title:", QLineEdit::Normal, title->text(), &ok);
  if (ok)
  {     
    title->setText(newTitle);
    graphTitle = newTitle;
    ui->customPlot->replot();
  }
}

/*void FenEnfantGraph::selectionChanged()
{
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelected(true);
    }
  }
}*/
//FenEnfantGraph parameters end

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

        ui->checkBoxDrawBetweenCursor->setEnabled(false);
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

        ui->checkBoxDrawBetweenCursor->setEnabled(true);
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
    Q_UNUSED(event)
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV(1, ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV(2, ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightMouved(QMouseEvent* event )
{
    Q_UNUSED(event)
    ui->customPlot->graph(indexGraph["cursor 1"])->clearData();
    setCursorCurveV(1, ui->curseur1->value(), indexGraph["cursor 1"], ui->customPlot->yAxis->range());
    emit cursor1Update();

    ui->customPlot->graph(indexGraph["cursor 2"])->clearData();
    setCursorCurveV(1, ui->curseur2->value(), indexGraph["cursor 2"], ui->customPlot->yAxis->range());
    emit cursor2Update();
}

void FenEnfantGraph::cursorHeightPressed(QMouseEvent* event )
{
    Q_UNUSED(event)
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
        ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
    }

    else if(stateZoomV == 0 && stateZoomH == 2)
    {
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
    }

    else if(stateZoomV == 2 && stateZoomH == 0)
    {
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
    }

    else if(stateZoomV == 0 && stateZoomH == 0)
    {
        ui->customPlot->axisRect()->setRangeZoom(0);
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
    ui->comboBoxCurveType->addItem("Filtre médian");

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

    ui->checkBoxDrawBetweenCursor->setCheckable(true);
    ui->checkBoxDrawBetweenCursor->setEnabled(false);
    ui->checkBoxDrawBetweenCursor->setChecked(false);
}

QString FenEnfantGraph::curveName(const QVector<double> &tabId)
{
    double opId(tabId.at(0));
    QString name;

    if(opId == 0)
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            name = tr("middle_%1_ST:%2").arg(ui->ComboBoxCurveName->currentText())
                                        .arg(ui->spinBoxSampleTime->value());
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            name = tr("middle_%1_ST:%2_%3->%4").arg(ui->ComboBoxCurveName->currentText())
                                               .arg(ui->spinBoxSampleTime->value())
                                               .arg(ui->curseur1->value())
                                               .arg(ui->curseur2->value());
        }

    }

    if(opId == 1)
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            name = tr("average_%1_ST:%2").arg(ui->ComboBoxCurveName->currentText())
                                .arg(ui->spinBoxSampleTime->value());
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            name = tr("middle_%1_ST:%2_%3->%4").arg(ui->ComboBoxCurveName->currentText())
                                               .arg(ui->spinBoxSampleTime->value())
                                               .arg(ui->curseur1->value())
                                               .arg(ui->curseur2->value());
        }
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
       if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
       {
           displayMathFunctionCurve(mathMethod->averageValueCurve(indexGraph[curveselected]+1, sampleTime));
       }
       else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
       {
           displayMathFunctionCurve(mathMethod->averageValueCurve(indexGraph[curveselected]+1, sampleTime, ui->curseur1->value(), ui->curseur2->value()));
       }
    }

    else if(curveType == "Filtre médian")
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            displayMathFunctionCurve(mathMethod->middleValueCurveFilter(indexGraph[curveselected]+1, sampleTime));
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            displayMathFunctionCurve(mathMethod->middleValueCurveFilter(indexGraph[curveselected]+1, sampleTime, ui->curseur1->value(), ui->curseur2->value()));
        }
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

//Export picture of graph
void FenEnfantGraph::exportGraphAsPng()
{
    QString outputDir = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.png)");
             QFile file(outputDir);

             if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
                  {
                      QMessageBox::warning(0,"Could not create Project File",
                                                 QObject::tr( "\n Could not create Project File on disk"));
                  }

     ui->customPlot->savePng(outputDir, ui->spinBoxYSizePng->value(), ui->spinBoxXSizePng->value(), 1.0, -1);
}

void FenEnfantGraph::exportGraphAsPdf()
{
    QString outputDir = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.pdf)");
             QFile file(outputDir);

             if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
                  {
                      QMessageBox::warning(0,"Could not create Project File",
                                                 QObject::tr( "\n Could not create Project File on disk"));
                  }

     ui->customPlot->savePdf(outputDir);
}

void FenEnfantGraph::exportGraphAsJpeg()
{
    QString outputDir = QFileDialog::getSaveFileName(this, "Enregistrer un fichier", QString(), "Images (*.jpg)");
             QFile file(outputDir);

             if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
                  {
                      QMessageBox::warning(0,"Could not create Project File",
                                                 QObject::tr( "\n Could not create Project File on disk"));
                  }

     ui->customPlot->saveJpg(outputDir, ui->spinBoxYSizeJpeg->value(), ui->spinBoxXSizeJpeg->value());
}

void FenEnfantGraph::linkPngValueX(const int &value)
{
    if (ui->pushButtonLinkPng->isChecked() == true && ui->spinBoxYSizePng->value() != ui->spinBoxXSizePng->value()*2)
    {
        ui->spinBoxXSizePng->setValue(value/2);
    }
}

void FenEnfantGraph::linkPngValueY(const int &value)
{
    if (ui->pushButtonLinkJpeg->isChecked() == true && ui->spinBoxXSizePng->value() != ui->spinBoxYSizePng->value()/2)
    {
        ui->spinBoxYSizePng->setValue(value*2);
    }
}

void FenEnfantGraph::linkJpegValueX(const int &value)
{
    if (ui->pushButtonLinkPng->isChecked() == true && ui->spinBoxYSizeJpeg->value() != ui->spinBoxXSizeJpeg->value()*2)
    {
        ui->spinBoxXSizeJpeg->setValue(value/2);
    }
}

void FenEnfantGraph::linkJpegValueY(const int &value)
{
    if (ui->pushButtonLinkJpeg->isChecked() == true && ui->spinBoxXSizeJpeg->value() != ui->spinBoxYSizeJpeg->value()/2)
    {
        ui->spinBoxYSizeJpeg->setValue(value*2);
    }
}
//Export picture of graph end

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

//Time absis management
void FenEnfantGraph::setGraphAbsisTime()
{
    if(ui->checkBoxTimeAbsis->checkState() == 2)
    {
        ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
        ui->customPlot->xAxis->setDateTimeSpec(Qt::UTC);
        ui->customPlot->xAxis->setDateTimeFormat("hh:mm:ss:zzz \n dd.MM.yyyy");
        addTimeOffsetToGraph();
        ui->customPlot->graph(0)->rescaleAxes();
        ui->customPlot->replot();

        ui->spinBoxTimeUnit->setEnabled(false);
        ui->comboBoxTimeUnit->setEnabled(false);
    }
    else if(ui->checkBoxTimeAbsis->checkState() == 0)
    {
        ui->customPlot->xAxis->setTickLabelType(QCPAxis::ltNumber);
        subTimeOffsetToGraph();
        ui->customPlot->graph(0)->rescaleAxes();
        ui->customPlot->replot();

        ui->spinBoxTimeUnit->setEnabled(true);
        ui->comboBoxTimeUnit->setEnabled(true);
    }
}

void FenEnfantGraph::calculateOffsetTime(const QStringList &fileInfo)
{
    //Convert date in second since 01/01/1970 00:00:00
    QDate startDate(1970, 1, 1);
    QDate date = QDate::fromString(fileInfo.at(2), "dd/MM/yyyy");

    QTime time = QTime::fromString(fileInfo.at(3), "hh:mm:ss");

    offsetTime = startDate.daysTo(date)*24*3600 + time.msecsSinceStartOfDay()/1000;
}

void FenEnfantGraph::addTimeOffsetToGraph()
{
    //Check time unit
    double unit(timeUnit());

    for(int j(0); j<= ui->customPlot->graphCount()-1; j++)
    {
        //Change value of keys
        QList<double> keysList(ui->customPlot->graph(j)->data()->keys());
        QVector<double> keysVector;

        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(keysList.at(i)*unit + offsetTime);
        }

        // extract values from QCPData and create a QVector of values
        QList<QCPData> valuesList(ui->customPlot->graph(j)->data()->values());
        QVector<double> valuesVector;
        for (int i(0); i <= valuesList.size()-1; i++)
    {
        valuesVector.push_back(valuesList.at(i).value);
    }

    ui->customPlot->graph(j)->setData(keysVector, valuesVector);
    }
}

void FenEnfantGraph::subTimeOffsetToGraph()
{
    //Check time unit
    double unit(timeUnit());

    for(int j(0); j<= ui->customPlot->graphCount()-1; j++)
    {
        //Change value of keys
        QList<double> keysList(ui->customPlot->graph(j)->data()->keys());
        QVector<double> keysVector;

        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back((keysList.at(i) - offsetTime)/unit);
        }

        // extract values from QCPData and create a QVector of values
        QList<QCPData> valuesList(ui->customPlot->graph(j)->data()->values());
        QVector<double> valuesVector;
        for (int i(0); i <= valuesList.size()-1; i++)
    {
        valuesVector.push_back(valuesList.at(i).value);
    }

    ui->customPlot->graph(j)->setData(keysVector, valuesVector);
    }
}

double FenEnfantGraph::timeUnit()
{
    double timeUnit;
    if (ui->comboBoxTimeUnit->currentText() == "s")
    {
        timeUnit = ui->spinBoxTimeUnit->value();
    }
    else if (ui->comboBoxTimeUnit->currentText() == "ms")
    {        
        timeUnit = (double) ui->spinBoxTimeUnit->value()/1000;
    }

    return timeUnit;
}

void FenEnfantGraph::changeAbsisName()
{
    QString timeAbsisLabel(m_header.at(0) + "(" + QString::number(ui->spinBoxTimeUnit->value()) + ui->comboBoxTimeUnit->currentText() + ")");
    ui->customPlot->xAxis->setLabel(timeAbsisLabel);
    ui->customPlot->replot();
}

//Time absis management end
