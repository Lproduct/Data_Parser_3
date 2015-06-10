#include "FenEnfantGraph.h"
#include "MathFunction.h"
#include "ui_FenEnfantGraph.h"
#include <algorithm>

FenEnfantGraph::FenEnfantGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FenEnfantGraph)
{
    ui->setupUi(this);

    ui->customPlot->replot();

    //NewCursor
    ui->checkBoxCurseurNew->setChecked(false);
    ui->spinBoxCurseur1->setEnabled(false);
    ui->spinBoxCurseur2->setEnabled(false);
    connect(ui->checkBoxCurseurNew, SIGNAL(stateChanged(int)), this, SLOT(cursorMangement(int)));

    signalMapperCursorV = new QSignalMapper(this);
    signalMapperCursorV->setMapping(ui->spinBoxCurseur1, 0);
    signalMapperCursorV->setMapping(ui->spinBoxCurseur2, 1);
    connect(ui->spinBoxCurseur1, SIGNAL(valueChanged(double)), signalMapperCursorV, SLOT(map()));
    connect(ui->spinBoxCurseur2, SIGNAL(valueChanged(double)), signalMapperCursorV, SLOT(map()));

    penCursor.setColor(QColor(255,170,100));
    penCursor.setWidth(2);
    penCursor.setStyle(Qt::DotLine);
    //NewCursor end

    //Tab curve list
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(curveDisplay(int)));

    //Zoom manager
    connect(ui->checkBoxZoomV, SIGNAL(stateChanged(int)), this, SLOT(zoom()));
    connect(ui->checkBoxZoomH, SIGNAL(stateChanged(int)), this, SLOT(zoom()));
    connect(ui->pushButtonAjustToScreen, SIGNAL(clicked()), this, SLOT(ajustToscreen()));

    //Create Curve
    connect(ui->pushButtoncreateCurve, SIGNAL(clicked()), this,SLOT(createCurve()));
    connect(ui->checkBoxCurseurNew, SIGNAL(clicked()), this, SLOT(drawBetweenCursorState()));

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

    //Custom curve
    ui->spinBoxCustomGain->setEnabled(false);
    ui->spinBoxCustomOffset->setEnabled(false);
    ui->pushButtonCustomCurve->setEnabled(false);
    connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(customCurveManagement()));

    //Del base line interaction
    penCursorInterpolZ1.setColor(Qt::red);
    penCursorInterpolZ1.setStyle(Qt::DotLine);
    penCursorInterpolZ1.setWidth(2);

    penCursorInterpolZ2.setColor(Qt::blue);
    penCursorInterpolZ2.setStyle(Qt::DotLine);
    penCursorInterpolZ2.setWidth(2);

    ui->comboBoxInterpolCurve->setEnabled(false);

    ui->checkBoxBaseLineEnable->setCheckable(true);
    ui->checkBoxBaseLineEnable->setChecked(false);

    ui->pushButtonLinkZone1->setCheckable(true);
    ui->pushButtonLinkZone1->setChecked(false);
    ui->pushButtonLinkZone1->setEnabled(false);

    ui->doubleSpinBoxCursorSpline1X->setEnabled(false);
    ui->doubleSpinBoxCursorSpline1X2->setEnabled(false);
    ui->doubleSpinBoxCursorSpline1Y->setEnabled(false);
    ui->doubleSpinBoxCursorSpline1Y2->setEnabled(false);

    ui->pushButtonLinkZone2->setCheckable(true);
    ui->pushButtonLinkZone2->setChecked(false);
    ui->pushButtonLinkZone2->setEnabled(false);

    ui->doubleSpinBoxCursorSpline2X->setEnabled(false);
    ui->doubleSpinBoxCursorSpline2X2->setEnabled(false);
    ui->doubleSpinBoxCursorSpline2Y->setEnabled(false);
    ui->doubleSpinBoxCursorSpline2Y2->setEnabled(false);

    ui->pushButtonDelBaseLine->setEnabled(false);

    ui->textEditPolyOrder->setEnabled(false);
    ui->textEditPolyOrder->setReadOnly(true);

    connect(ui->checkBoxBaseLineEnable, SIGNAL(stateChanged(int)), this, SLOT(interpolationInteraction(int)));
    connect(ui->checkBoxBaseLineEnable,SIGNAL(stateChanged(int)), this, SLOT(cursorMangementInterpol(int)));

    connect(ui->pushButtonLinkZone1, SIGNAL(clicked(bool)), this, SLOT(linkCursorZ1(bool)));
    connect(ui->pushButtonLinkZone1, SIGNAL(clicked(bool)), this, SLOT(linkCursorZ2(bool)));

        //cursor spinbox interaction
    ui->spinBoxSampleTime->setVisible(false);
    ui->label_percent->setVisible(false);
    ui->label_sampleTime->setVisible(false);
    connect(ui->comboBoxCurveType, SIGNAL(activated(QString)), this, SLOT(cursorSpinBoxInt(QString)));

        //link vetical cursor to signal mapper
    signalMapperCursorV->setMapping(ui->doubleSpinBoxCursorSpline1X, 2);
    signalMapperCursorV->setMapping(ui->doubleSpinBoxCursorSpline1X2, 3);
    signalMapperCursorV->setMapping(ui->doubleSpinBoxCursorSpline2X, 4);
    signalMapperCursorV->setMapping(ui->doubleSpinBoxCursorSpline2X2, 5);

    connect(ui->doubleSpinBoxCursorSpline1X, SIGNAL(valueChanged(double)), signalMapperCursorV, SLOT(map()));
    connect(ui->doubleSpinBoxCursorSpline1X2, SIGNAL(valueChanged(double)), signalMapperCursorV, SLOT(map()));
    connect(ui->doubleSpinBoxCursorSpline2X, SIGNAL(valueChanged(double)), signalMapperCursorV, SLOT(map()));
    connect(ui->doubleSpinBoxCursorSpline2X2, SIGNAL(valueChanged(double)), signalMapperCursorV, SLOT(map()));

    signalMapperCursorH = new QSignalMapper(this);
    signalMapperCursorH->setMapping(ui->doubleSpinBoxCursorSpline1Y, 0);
    signalMapperCursorH->setMapping(ui->doubleSpinBoxCursorSpline1Y2, 1);
    signalMapperCursorH->setMapping(ui->doubleSpinBoxCursorSpline2Y, 2);
    signalMapperCursorH->setMapping(ui->doubleSpinBoxCursorSpline2Y2, 3);

    connect(ui->doubleSpinBoxCursorSpline1Y, SIGNAL(valueChanged(double)), signalMapperCursorH, SLOT(map()));
    connect(ui->doubleSpinBoxCursorSpline1Y2, SIGNAL(valueChanged(double)), signalMapperCursorH, SLOT(map()));
    connect(ui->doubleSpinBoxCursorSpline2Y, SIGNAL(valueChanged(double)), signalMapperCursorH, SLOT(map()));
    connect(ui->doubleSpinBoxCursorSpline2Y2, SIGNAL(valueChanged(double)), signalMapperCursorH, SLOT(map()));

        //Regression interaction
    connect(ui->pushButtonDelBaseLine, SIGNAL(clicked()), this, SLOT(delBaseLine()));
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
    addItemToComboboxInterpol();
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
          contextMenu->addAction("Change curve style", this, SLOT(changeSelectedGraphStyle()));
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

void FenEnfantGraph::changeSelectedGraphColor()
{
    QPen newPen;

    QPen pen(ui->customPlot->selectedGraphs().first()->pen());
    newPen.setWidth(pen.width());
    newPen.setStyle(pen.style());

    QColor newColor = QColorDialog::getColor(Qt::black, this);

    newPen.setColor(newColor);

    ui->customPlot->selectedGraphs().first()->setPen(newPen);
    ui->customPlot->replot();
}

void FenEnfantGraph::changeSelectedGraphThickness()
{
    QPen newPen;

    QPen pen(ui->customPlot->selectedGraphs().first()->pen());
    newPen.setColor(pen.color());
    newPen.setStyle(pen.style());

    int newValue = QInputDialog::getInt(this, "Data Parser", "New curve thickness:", 0, 0, 100);

    newPen.setWidth(newValue);

    ui->customPlot->selectedGraphs().first()->setPen(newPen);
    ui->customPlot->replot();
}

void FenEnfantGraph::changeSelectedGraphScatter()
{
    QPen newPen;

    QPen pen(ui->customPlot->selectedGraphs().first()->pen());
    newPen.setColor(pen.color());
    newPen.setWidth(pen.width());
    newPen.setStyle(pen.style());

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

    ui->customPlot->selectedGraphs().first()->setPen(newPen);
    ui->customPlot->replot();
}

void FenEnfantGraph::changeSelectedGraphStyle()
{
    QPen newPen;

    QPen pen(ui->customPlot->selectedGraphs().first()->pen());
    newPen.setColor(pen.color());
    newPen.setWidth(pen.width());

    QStringList item;
    item.push_back("None");
    item.push_back("SolidLine");
    item.push_back("DashLine");
    item.push_back("DotLine");
    item.push_back("DashDotLine");
    item.push_back("DashDotDotLine");
    QString newItem = QInputDialog::getItem(this, "Data Parser", "New curve style:",item, 0, false);

    if (newItem == "None")
    {
        newPen.setStyle(Qt::NoPen);
    }
    else if (newItem == "SolidLine")
    {
        newPen.setStyle(Qt::SolidLine);
    }
    else if (newItem == "DashLine")
    {
        newPen.setStyle(Qt::DashLine);
    }
    else if (newItem == "DotLine")
    {
        newPen.setStyle(Qt::DotLine);
    }
    else if (newItem == "DashDotLine")
    {
        newPen.setStyle(Qt::DashDotLine);
    }
    else if (newItem == "DashDotDotLine")
    {
        newPen.setStyle(Qt::DashDotDotLine);
    }

    ui->customPlot->selectedGraphs().first()->setPen(newPen);
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

//FenEnfantGraph parameters end

//New Cursor
void FenEnfantGraph::cursorMangement(const int &state)
{
    if(state == 2)
    {
        controlPressed = false;

        ui->checkBoxBaseLineEnable->setChecked(false);

        QVector<double> dataInfo = mathMethod->dataInfo();
        ui->spinBoxCurseur1->setMinimum(dataInfo.at(0));
        ui->spinBoxCurseur1->setMaximum(dataInfo.at(1));
        ui->spinBoxCurseur1->setSingleStep(dataInfo.at(2));

        ui->spinBoxCurseur2->setMinimum(dataInfo.at(0));
        ui->spinBoxCurseur2->setMaximum(dataInfo.at(1));
        ui->spinBoxCurseur2->setSingleStep(dataInfo.at(2));

        createCursorNew();

    }
    else if (state == 0)
    {
        killCursorNew();
        ui->spinBoxCurseur1->setEnabled(false);
        ui->spinBoxCurseur2->setEnabled(false);
    }
}

void FenEnfantGraph::createCursorNew()
{
    double incrementH((mathMethod->dataInfo().at(1))/100);

    double viewCenter (ui->customPlot->xAxis->range().center());

    if(ui->checkBoxTimeAbsis->checkState() == 0)
    {
        ui->spinBoxCurseur1->setValue((int) viewCenter -incrementH*5);
        ui->spinBoxCurseur2->setValue((int) viewCenter +incrementH*5);
    }
    else if(ui->checkBoxTimeAbsis->checkState() == 2)
    {
        ui->spinBoxCurseur1->setValue((int) viewCenter -incrementH*5 - offsetTime);
        ui->spinBoxCurseur2->setValue((int) viewCenter +incrementH*5 - offsetTime);
    }

    setCursorVNew("cursorNew 1", viewCenter -incrementH*5, penCursor);
    setCursorVNew("cursorNew 2", viewCenter +incrementH*5, penCursor);
    ui->spinBoxCurseur1->setEnabled(true);
    ui->spinBoxCurseur2->setEnabled(true);
    createCursorNewConnection();
}

void FenEnfantGraph::createCursorNewConnection()
{
    connect(signalMapperCursorV, SIGNAL(mapped(int)), this, SLOT(moveCursorV(int)));
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(resizeCursorScroll(QWheelEvent*)));
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(resizeCursorMouse(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(resizeCursorMouse(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(resizeCursorMouse(QMouseEvent*)));
}

void FenEnfantGraph::killCursorNew()
{
    ui->customPlot->removeGraph(indexGraph["cursorNew 2"]);
    ui->customPlot->removeGraph(indexGraph["cursorNew 1"]);
    ui->customPlot->replot();

    //erase cursor from indexGraph
    eraseGraphNameFromIndex("cursorNew 1");
    eraseGraphNameFromIndex("cursorNew 2");

    killCursorNewConnection();
}

void FenEnfantGraph::killCursorNewConnection()
{
    disconnect(signalMapperCursorV, SIGNAL(mapped(int)), this, SLOT(moveCursorV(int)));
    disconnect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(resizeCursorScroll(QWheelEvent*)));
    disconnect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(resizeCursorMouse(QMouseEvent*)));
    disconnect(ui->customPlot, SIGNAL(mouseRelease(QMouseEvent*)), this, SLOT(resizeCursorMouse(QMouseEvent*)));
    disconnect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(resizeCursorMouse(QMouseEvent*)));
}

void FenEnfantGraph::setCursorVNew(const QString &cursorName, const double &posCursor, const QPen &pen, const double &offset)
{
    QVector<double> absTab;
    for(int i(0); i <= 1; i++)
    {
        absTab.push_back(posCursor + offset);
    }

    QVector<double> ordTab;
    ordTab.push_back(ui->customPlot->yAxis->range().lower);
    ordTab.push_back(ui->customPlot->yAxis->range().upper);

    //Draw cursor
    displayCursor(cursorName, absTab, ordTab, pen);
}

void FenEnfantGraph::displayCursor(const QString &name, const QVector<double> &absTab, const QVector<double> &ordTab, const QPen &pen)
{
    int nbGraph(ui->customPlot->graphCount());

    ui->customPlot->addGraph();
    ui->customPlot->graph(nbGraph)->setPen(pen);
    ui->customPlot->graph(nbGraph)->setName(name);
    ui->customPlot->graph(nbGraph)->setData(absTab, ordTab);
    ui->customPlot->replot();

    //setTabCurve(name);
    indexGraph[name] = (nbGraph);
}

void FenEnfantGraph::eraseGraphNameFromIndex(const QString &name)
{
    for(std::map<QString,int>::iterator it(indexGraph.begin()); it !=indexGraph.end(); it++)
    {
        if(it->first == name)
        {
            indexGraph.erase(it);
            break;
        }
    }
}

void FenEnfantGraph::moveCursorV(const int &id)
{
    QString name;
    double value;
    if (id == 0)
    {
        name = "cursorNew 1";
        value = ui->spinBoxCurseur1->value();
    }
    else if (id == 1)
    {
         name = "cursorNew 2";
         value = ui->spinBoxCurseur2->value();
    }
    else if (id == 2)
    {
         name = "cursorInterpol 1Z1";
         value = ui->doubleSpinBoxCursorSpline1X->value();
    }
    else if (id == 3)
    {
         name = "cursorInterpol 2Z1";
         value = ui->doubleSpinBoxCursorSpline1X2->value();
    }
    else if (id == 4)
    {
         name = "cursorInterpol 1Z2";
         value = ui->doubleSpinBoxCursorSpline2X->value();
    }
    else if (id == 5)
    {
         name = "cursorInterpol 2Z2";
         value = ui->doubleSpinBoxCursorSpline2X2->value();
    }

    //Change value of keys
    QList<double> keysList(ui->customPlot->graph(indexGraph[name])->data()->keys());
    QVector<double> keysVector;

    if(ui->checkBoxTimeAbsis->checkState() == 0)
    {
        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(value);
        }
    }
    else if (ui->checkBoxTimeAbsis->checkState() == 2)
    {
        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(value*timeUnit() + offsetTime);
        }
    }

    // extract values from QCPData and create a QVector of values
    QList<QCPData> valuesList(ui->customPlot->graph(indexGraph[name])->data()->values());
    QVector<double> valuesVector;
    for (int i(0); i <= valuesList.size()-1; i++)
    {
        valuesVector.push_back(valuesList.at(i).value);
    }

    ui->customPlot->graph(indexGraph[name])->setData(keysVector, valuesVector);
    ui->customPlot->replot();
}

void FenEnfantGraph::resizeCursorScroll(const QWheelEvent* &event)
{
    Q_UNUSED(event)

    sizeCursorV("cursorNew 1", ui->spinBoxCurseur1);
    sizeCursorV("cursorNew 2", ui->spinBoxCurseur2);
}

void FenEnfantGraph::resizeCursorMouse(const QMouseEvent* &event)
{
    Q_UNUSED(event)

    sizeCursorV("cursorNew 1", ui->spinBoxCurseur1);
    sizeCursorV("cursorNew 2", ui->spinBoxCurseur2);
}

void FenEnfantGraph::sizeCursorV(const QString &name, QDoubleSpinBox *spinbox)
{
    double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    double value(0);
    if (ui->checkBoxTimeAbsis->checkState() == 0)
    {
        value = spinbox->value();
    }
    else if (ui->checkBoxTimeAbsis->checkState() == 2)
    {
        value = spinbox->value()*timeUnit() + offsetTime;
    }

    //Change value of keys
    QList<double> keysList(ui->customPlot->graph(indexGraph[name])->data()->keys());
    QVector<double> keysVector;

    for (int i(0); i <= keysList.size()-1; i++)
    {
        keysVector.push_back(value);
    }


    // extract values from QCPData and create a QVector of values
    QVector<double> valuesVector;
    if(name == "cursorInterpol 1Z1")
    {
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline1Y->value() - incrementV);
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline1Y2->value() + incrementV);
    }
    else if(name == "cursorInterpol 2Z1")
    {
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline1Y->value()- incrementV);
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline1Y2->value() + incrementV);
    }
    else if(name == "cursorInterpol 1Z2")
    {
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline2Y->value() - incrementV);
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline2Y2->value() + incrementV);
    }
    else if(name == "cursorInterpol 2Z2")
    {
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline2Y->value() - incrementV);
        valuesVector.push_back(ui->doubleSpinBoxCursorSpline2Y2->value() + incrementV);
    }
    else
    {
        valuesVector.push_back(ui->customPlot->yAxis->range().lower);
        valuesVector.push_back(ui->customPlot->yAxis->range().upper);
    }

    ui->customPlot->graph(indexGraph[name])->setData(keysVector, valuesVector);
    ui->customPlot->replot();
}

void FenEnfantGraph::keyPressEvent(QKeyEvent *event)
{
    /*double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    double incrementH((mathMethod->dataInfo().at(1))/100);*/

    double incrementV;
    double incrementH;
    if(ui->checkBoxTimeAbsis->checkState() == 0)
    {
        incrementV = (ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower)/100;
        incrementH = (ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower)/100;
    }
    else if (ui->checkBoxTimeAbsis->checkState() == 2)
    {
        incrementV = (ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower)/100;
        incrementH = (ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower)/100;
    }


    if(ui->checkBoxCurseurNew->checkState() == 2 || ui->checkBoxBaseLineEnable->checkState() == 2)
    {
        if(event->key() ==  Qt::Key_Control)
        {
            controlPressed = true;
        }
        else if (controlPressed == false)
        {
            if(event->key() == Qt::Key_Plus)
            {
                if(ui->customPlot->graph(indexGraph["cursorNew 1"])->selected() == true)
                {
                    ui->spinBoxCurseur1->setValue(ui->spinBoxCurseur1->value()+incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorNew 2"])->selected() == true)
                {
                   ui->spinBoxCurseur2->setValue(ui->spinBoxCurseur2->value()+incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X->setValue(ui->doubleSpinBoxCursorSpline1X->value()+incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X2->setValue(ui->doubleSpinBoxCursorSpline1X2->value()+incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X->setValue(ui->doubleSpinBoxCursorSpline2X->value()+incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X2->setValue(ui->doubleSpinBoxCursorSpline2X2->value()+incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y->setValue(ui->doubleSpinBoxCursorSpline1Y->value()+incrementV);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y2->setValue(ui->doubleSpinBoxCursorSpline1Y2->value()+incrementV);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y->setValue(ui->doubleSpinBoxCursorSpline2Y->value()+incrementV);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y2->setValue(ui->doubleSpinBoxCursorSpline2Y2->value()+incrementV);
                }
            }
            else if(event->key() == Qt::Key_Minus)
            {
                if(ui->customPlot->graph(indexGraph["cursorNew 1"])->selected() == true)
                {
                   ui->spinBoxCurseur1->setValue(ui->spinBoxCurseur1->value()-incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorNew 2"])->selected() == true)
                {
                   ui->spinBoxCurseur2->setValue(ui->spinBoxCurseur2->value()-incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X->setValue(ui->doubleSpinBoxCursorSpline1X->value()-incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X2->setValue(ui->doubleSpinBoxCursorSpline1X2->value()-incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X->setValue(ui->doubleSpinBoxCursorSpline2X->value()-incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X2->setValue(ui->doubleSpinBoxCursorSpline2X2->value()-incrementH);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y->setValue(ui->doubleSpinBoxCursorSpline1Y->value()-incrementV);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y2->setValue(ui->doubleSpinBoxCursorSpline1Y2->value()-incrementV);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y->setValue(ui->doubleSpinBoxCursorSpline2Y->value()-incrementV);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y2->setValue(ui->doubleSpinBoxCursorSpline2Y2->value()-incrementV);
                }
            }
        }

        else if(controlPressed == true)
        {
            if(event->key() == (Qt::Key_Minus))
            {
                if(ui->customPlot->graph(indexGraph["cursorNew 1"])->selected() == true)
                {
                   ui->spinBoxCurseur1->setValue(ui->spinBoxCurseur1->value()-incrementH*10);
                }

                if(ui->customPlot->graph(indexGraph["cursorNew 2"])->selected() == true)
                {
                   ui->spinBoxCurseur2->setValue(ui->spinBoxCurseur2->value()-incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X->setValue(ui->doubleSpinBoxCursorSpline1X->value()-incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X2->setValue(ui->doubleSpinBoxCursorSpline1X2->value()-incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X->setValue(ui->doubleSpinBoxCursorSpline2X->value()-incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X2->setValue(ui->doubleSpinBoxCursorSpline2X2->value()-incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y->setValue(ui->doubleSpinBoxCursorSpline1Y->value()-incrementV*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y2->setValue(ui->doubleSpinBoxCursorSpline1Y2->value()-incrementV*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y->setValue(ui->doubleSpinBoxCursorSpline2Y->value()-incrementV*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y2->setValue(ui->doubleSpinBoxCursorSpline2Y2->value()-incrementV*10);
                }
            }
            else if(event->key() == (Qt::Key_Plus))
            {
                if(ui->customPlot->graph(indexGraph["cursorNew 1"])->selected() == true)
                {
                   ui->spinBoxCurseur1->setValue(ui->spinBoxCurseur1->value()+incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorNew 2"])->selected() == true)
                {
                   ui->spinBoxCurseur2->setValue(ui->spinBoxCurseur2->value()+incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X->setValue(ui->doubleSpinBoxCursorSpline1X->value()+incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1X2->setValue(ui->doubleSpinBoxCursorSpline1X2->value()+incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X->setValue(ui->doubleSpinBoxCursorSpline2X->value()+incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2X2->setValue(ui->doubleSpinBoxCursorSpline2X2->value()+incrementH*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y->setValue(ui->doubleSpinBoxCursorSpline1Y->value()+incrementV*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z1 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline1Y2->setValue(ui->doubleSpinBoxCursorSpline1Y2->value()+incrementV*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 1Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y->setValue(ui->doubleSpinBoxCursorSpline2Y->value()+incrementV*10);
                }

                else if(ui->customPlot->graph(indexGraph["cursorInterpol 2Z2 H"])->selected() == true)
                {
                   ui->doubleSpinBoxCursorSpline2Y2->setValue(ui->doubleSpinBoxCursorSpline2Y2->value()+incrementV*10);
                }
            }
        }
    }
}

void FenEnfantGraph::keyReleaseEvent(QKeyEvent *event)
{
    if(ui->checkBoxCurseurNew->checkState() == 2 || ui->checkBoxBaseLineEnable->checkState() == 2)
    {
        if(event->key() ==  Qt::Key_Control)
        {
            controlPressed = false;
        }
    }
}
//New Cursor end

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

void FenEnfantGraph::ajustToscreen()
{
    if(ui->customPlot->selectedGraphs().size()!=0)
    {
        ui->customPlot->selectedGraphs().first()->rescaleAxes();
        ui->customPlot->replot();
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
    ui->comboBoxCurveType->addItem("None");
    ui->comboBoxCurveType->addItem("Moyenne");
    ui->comboBoxCurveType->addItem("Mouving Average");
    ui->comboBoxCurveType->addItem("Filtre médian");
    ui->comboBoxCurveType->addItem("Mouving Median");
    ui->comboBoxCurveType->addItem("FFT Filter");
    ui->ComboBoxCurveName->setEnabled(true);

    for(std::map<QString,int>::iterator it(indexGraph.begin()); it!= indexGraph.end(); it++)
    {
        QString nameCurve(it->first);
        if (nameCurve != "cursorNew 1" || nameCurve != "cursorNew 2")
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
                                               .arg(ui->spinBoxCurseur1->value())
                                               .arg(ui->spinBoxCurseur2->value());
        }

    }

    else if(opId == 1)
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            name = tr("average_%1_ST:%2").arg(ui->ComboBoxCurveName->currentText())
                                .arg(ui->spinBoxSampleTime->value());
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            name = tr("average_%1_ST:%2_%3->%4").arg(ui->ComboBoxCurveName->currentText())
                                               .arg(ui->spinBoxSampleTime->value())
                                               .arg(ui->spinBoxCurseur1->value())
                                               .arg(ui->spinBoxCurseur2->value());
        }
    }

    else if(opId == 2)
    {
        QString nameGraph(ui->customPlot->selectedGraphs().first()->name());
            name = nameGraph + tr("_G:%1_Off:%2").arg(ui->spinBoxCustomGain->value())
                                                 .arg(ui->spinBoxCustomOffset->value());
    }

    else if(opId == 3)
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            name = tr("FFT_Filt_%1_%2%").arg(ui->ComboBoxCurveName->currentText())
                                .arg(ui->spinBoxSampleTime->value());
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            name = tr("FFT_Filt_%1_%2%_%3->%4").arg(ui->ComboBoxCurveName->currentText())
                                               .arg(ui->spinBoxSampleTime->value())
                                               .arg(ui->spinBoxCurseur1->value())
                                               .arg(ui->spinBoxCurseur2->value());
        }
    }

    else if(opId == 4)
    {
        name = tr("Point");
    }

    else if(opId == 5)
    {
        name = tr("Spline_%1").arg(ui->comboBoxInterpolCurve->currentText());
    }

    else if(opId == 6)
    {
        name = tr("Del_Base_Line_%1").arg(ui->comboBoxInterpolCurve->currentText());
    }

    else if(opId == 7)
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            name = tr("mouving_middle_%1_ST:%2").arg(ui->ComboBoxCurveName->currentText())
                                        .arg(ui->spinBoxSampleTime->value());
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            name = tr("mouving_middle_%1_ST:%2_%3->%4").arg(ui->ComboBoxCurveName->currentText())
                                               .arg(ui->spinBoxSampleTime->value())
                                               .arg(ui->spinBoxCurseur1->value())
                                               .arg(ui->spinBoxCurseur2->value());
        }
    }

    else if(opId == 8)
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            name = tr("mouving_median_%1_ST:%2").arg(ui->ComboBoxCurveName->currentText())
                                        .arg(ui->spinBoxSampleTime->value());
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            name = tr("mouving_median_%1_ST:%2_%3->%4").arg(ui->ComboBoxCurveName->currentText())
                                               .arg(ui->spinBoxSampleTime->value())
                                               .arg(ui->spinBoxCurseur1->value())
                                               .arg(ui->spinBoxCurseur2->value());
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
           displayMathFunctionCurve(mathMethod->averageValueCurveNew(indexGraph[curveselected]+1, sampleTime));
       }
       else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
       {
           displayMathFunctionCurve(mathMethod->averageValueCurveNew(indexGraph[curveselected]+1, sampleTime, ui->spinBoxCurseur1->value(), ui->spinBoxCurseur2->value()));
       }
    }

    else if(curveType == "Mouving Average")
    {
       if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
       {
           displayMathFunctionCurve(mathMethod->mouvingAverageValueCurve(indexGraph[curveselected]+1, sampleTime));
       }
       else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
       {
           displayMathFunctionCurve(mathMethod->mouvingAverageValueCurve(indexGraph[curveselected]+1, sampleTime, ui->spinBoxCurseur1->value(), ui->spinBoxCurseur2->value()));
       }
    }

    else if(curveType == "Filtre médian")
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            displayMathFunctionCurve(mathMethod->middleValueCurveFilterNew(indexGraph[curveselected]+1, sampleTime));
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            displayMathFunctionCurve(mathMethod->middleValueCurveFilterNew(indexGraph[curveselected]+1, sampleTime, ui->spinBoxCurseur1->value(), ui->spinBoxCurseur2->value()));
        }
    }


    else if(curveType == "Mouving Median")
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            displayMathFunctionCurve(mathMethod->mouvingMedianValueCurve(indexGraph[curveselected]+1, sampleTime));
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            displayMathFunctionCurve(mathMethod->mouvingMedianValueCurve(indexGraph[curveselected]+1, sampleTime, ui->spinBoxCurseur1->value(), ui->spinBoxCurseur2->value()));
        }
    }

    else if(curveType == "FFT Filter")
    {
        if (ui->checkBoxDrawBetweenCursor->checkState() == 0)
        {
            displayMathFunctionCurve(mathMethod->fftFilter(indexGraph[curveselected]+1, sampleTime));
        }
        else if (ui->checkBoxDrawBetweenCursor->checkState() == 2)
        {
            displayMathFunctionCurve(mathMethod->fftFilter(indexGraph[curveselected]+1, sampleTime, ui->spinBoxCurseur1->value(), ui->spinBoxCurseur2->value()));
        }
    }

    ui->customPlot->replot();
}

void FenEnfantGraph::displayMathFunctionCurve(const QVector<QVector<double> > &tab)
{
    //if cursor exist, Kill it
    if(ui->checkBoxCurseurNew->checkState() == 2)
    {
        killCursorNew();
    }

    else if(ui->checkBoxBaseLineEnable->checkState() == 2)
    {
        killCursorInterpol();
    }

    //insert curve
    int nbGraph(ui->customPlot->graphCount());
    ui->customPlot->addGraph();
    ui->customPlot->graph(nbGraph)->setPen(QPen(randomColor("normal")));
    ui->customPlot->graph(nbGraph)->setName(curveName(tab.at(2)));
    ui->customPlot->graph(nbGraph)->setData(tab.at(0), tab.at(1));

    //rescale curve if time offset is on
    if(ui->checkBoxTimeAbsis->checkState() == 2)
    {
        //Check time unit
        double unit(timeUnit());

        //Change value of keys
        QList<double> keysList(ui->customPlot->graph(nbGraph)->data()->keys());
        QVector<double> keysVector;

        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(keysList.at(i)*unit + offsetTime);
        }

        // extract values from QCPData and create a QVector of values
        QList<QCPData> valuesList(ui->customPlot->graph(nbGraph)->data()->values());
        QVector<double> valuesVector;
        for (int i(0); i <= valuesList.size()-1; i++)
        {
            valuesVector.push_back(valuesList.at(i).value);
        }

        ui->customPlot->graph(nbGraph)->setData(keysVector, valuesVector);
    }

    setTabCurve(curveName(tab.at(2)));
    indexGraph[curveName(tab.at(2))] = ui->customPlot->graphCount()-1;

    //if cursor previously exist set Cursor
    if(ui->checkBoxCurseurNew->checkState() == 2)
    {
        setCursorVNew("cursorNew 1", ui->spinBoxCurseur1->value(), penCursor);
        setCursorVNew("cursorNew 2", ui->spinBoxCurseur2->value(), penCursor);
        createCursorNewConnection();
    }

    else if(ui->checkBoxBaseLineEnable->checkState() == 2)
    {
        //zone 1
        setCursorV("cursorInterpol 1Z1", ui->doubleSpinBoxCursorSpline1X->value(), penCursorInterpolZ1);
        setCursorV("cursorInterpol 2Z1", ui->doubleSpinBoxCursorSpline1X2->value(), penCursorInterpolZ1);
        setCursorH("cursorInterpol 1Z1 H", ui->doubleSpinBoxCursorSpline1Y->value(), penCursorInterpolZ1);
        setCursorH("cursorInterpol 2Z1 H", ui->doubleSpinBoxCursorSpline1Y2->value(), penCursorInterpolZ1);

        //zone 2
        setCursorV("cursorInterpol 1Z2", ui->doubleSpinBoxCursorSpline2X->value(), penCursorInterpolZ2);
        setCursorV("cursorInterpol 2Z2", ui->doubleSpinBoxCursorSpline2X2->value(), penCursorInterpolZ2);
        setCursorH("cursorInterpol 1Z2 H", ui->doubleSpinBoxCursorSpline2Y->value(), penCursorInterpolZ2);
        setCursorH("cursorInterpol 2Z2 H", ui->doubleSpinBoxCursorSpline2Y2->value(), penCursorInterpolZ2);
        createCursorInterpolConnection();
    }
}

void FenEnfantGraph::drawBetweenCursorState()
{
    if (ui->checkBoxCurseurNew->checkState() == 2)
    {
        ui->checkBoxDrawBetweenCursor->setEnabled(true);
    }
    else if (ui->checkBoxCurseurNew->checkState() == 0)
    {
        ui->checkBoxDrawBetweenCursor->setEnabled(false);
        ui->checkBoxDrawBetweenCursor->setChecked(false);
    }
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
    double timeUnit(1);
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

//Custom Curve
void FenEnfantGraph::customCurveManagement()
{
    if(ui->customPlot->selectedGraphs().size() > 0)
    {
        ui->spinBoxCustomGain->setEnabled(true);
        ui->spinBoxCustomOffset->setEnabled(true);
        ui->pushButtonCustomCurve->setEnabled(true);
        connect(ui->pushButtonCustomCurve, SIGNAL(clicked()), this,SLOT(customCurve()));
    }
    else
    {
        ui->spinBoxCustomGain->setEnabled(false);
        ui->spinBoxCustomOffset->setEnabled(false);
        ui->pushButtonCustomCurve->setEnabled(false);
        disconnect(ui->pushButtonCustomCurve, SIGNAL(clicked()), this,SLOT(customCurve()));
    }
}

void FenEnfantGraph::customCurve()
{
    double gain(ui->spinBoxCustomGain->value());
    int offset(ui->spinBoxCustomOffset->value());

    //Change value of keys
    QList<double> keysList(ui->customPlot->selectedGraphs().first()->data()->keys());
    QVector<double> keysVector;

    if(ui->checkBoxTimeAbsis->checkState() == 2)
    {
        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(keysList.at(i)*timeUnit() + offset*timeUnit() - offsetTime);
        }
    }
    else if(ui->checkBoxTimeAbsis->checkState() == 0)
    {
        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(keysList.at(i) + offset);
        }
    }

    // extract values from QCPData and create a QVector of values
    QList<QCPData> valuesList(ui->customPlot->selectedGraphs().first()->data()->values());
    QVector<double> valuesVector;
    for (int i(0); i <= valuesList.size()-1; i++)
    {
        valuesVector.push_back(valuesList.at(i).value*gain);
    }

    QVector<double> tabId;
    tabId.push_back(2);
    QVector<QVector<double> > tab;
    tab.push_back(keysVector);
    tab.push_back(valuesVector);
    tab.push_back(tabId);

    displayMathFunctionCurve(tab);

    ui->customPlot->replot();
}
//Custom Curve end

//cursor spinbox interaction
void FenEnfantGraph::cursorSpinBoxInt(const QString &choice)
{
    QVector<double> dataInfo = mathMethod->dataInfo();
    if(choice == "None")
    {
        ui->spinBoxSampleTime->setVisible(false);
        ui->label_sampleTime->setVisible(false);
        ui->label_percent->setVisible(false);
    }
    else if(choice == "Moyenne")
    {
        ui->spinBoxSampleTime->setVisible(true);
        ui->label_sampleTime->setVisible(true);
        ui->label_percent->setVisible(false);
        ui->spinBoxSampleTime->setMinimum(1);
        ui->spinBoxSampleTime->setMaximum(dataInfo.at(3)/2);
        ui->doubleSpinBoxCursorSpline1Y->setSingleStep(1);
        ui->doubleSpinBoxCursorSpline1Y->setValue(1);
    }
    else if(choice == "Filtre médian")
    {
        ui->spinBoxSampleTime->setVisible(true);
        ui->label_sampleTime->setVisible(true);
        ui->label_percent->setVisible(false);
        ui->spinBoxSampleTime->setMinimum(1);
        ui->spinBoxSampleTime->setMaximum(dataInfo.at(3)/2);
        ui->doubleSpinBoxCursorSpline1Y->setSingleStep(2);
        ui->doubleSpinBoxCursorSpline1Y->setValue(1);
    }
    else if(choice == "FFT Filter")
    {
        ui->spinBoxSampleTime->setVisible(true);
        ui->label_sampleTime->setVisible(false);
        ui->label_percent->setVisible(true);
        ui->spinBoxSampleTime->setMinimum(0);
        ui->spinBoxSampleTime->setMaximum(100);
        ui->doubleSpinBoxCursorSpline1Y->setSingleStep(0.1);
        ui->doubleSpinBoxCursorSpline1Y->setValue(0);
    }
    else if(choice == "Mouving Average")
    {
        ui->spinBoxSampleTime->setVisible(true);
        ui->label_sampleTime->setVisible(true);
        ui->label_percent->setVisible(false);
        ui->spinBoxSampleTime->setMinimum(1);
        ui->spinBoxSampleTime->setMaximum(dataInfo.at(3)/2);
        ui->doubleSpinBoxCursorSpline1Y->setSingleStep(2);
        ui->doubleSpinBoxCursorSpline1Y->setValue(1);
    }
    else if(choice == "Mouving Median")
    {
        ui->spinBoxSampleTime->setVisible(true);
        ui->label_sampleTime->setVisible(true);
        ui->label_percent->setVisible(false);
        ui->spinBoxSampleTime->setMinimum(1);
        ui->spinBoxSampleTime->setMaximum(dataInfo.at(3)/2);
        ui->doubleSpinBoxCursorSpline1Y->setSingleStep(2);
        ui->doubleSpinBoxCursorSpline1Y->setValue(1);
    }
}
//cursor spinbox interaction end

//Del base line interaction
void FenEnfantGraph::interpolationInteraction(const int &state)
{
    if (state == 2)
    {
        //addItemToComboboxInterpol();

        ui->pushButtonLinkZone1->setCheckable(true);
        ui->pushButtonLinkZone1->setChecked(false);
        ui->pushButtonLinkZone1->setEnabled(true);

        ui->doubleSpinBoxCursorSpline1X->setEnabled(true);
        ui->doubleSpinBoxCursorSpline1X2->setEnabled(true);
        ui->doubleSpinBoxCursorSpline1Y->setEnabled(true);
        ui->doubleSpinBoxCursorSpline1Y2->setEnabled(true);

        ui->pushButtonLinkZone2->setCheckable(true);
        ui->pushButtonLinkZone2->setChecked(false);
        ui->pushButtonLinkZone2->setEnabled(true);

        ui->doubleSpinBoxCursorSpline2X->setEnabled(true);
        ui->doubleSpinBoxCursorSpline2X2->setEnabled(true);
        ui->doubleSpinBoxCursorSpline2Y->setEnabled(true);
        ui->doubleSpinBoxCursorSpline2Y2->setEnabled(true);

        ui->pushButtonDelBaseLine->setEnabled(true);

        ui->textEditPolyOrder->setEnabled(true);

        ui->comboBoxInterpolCurve->setEnabled(true);
    }

    else if (state == 0)
    {
        //destroyItemFromCombobox();

        ui->pushButtonLinkZone1->setCheckable(true);
        ui->pushButtonLinkZone1->setChecked(false);
        ui->pushButtonLinkZone1->setEnabled(false);

        ui->doubleSpinBoxCursorSpline1X->setEnabled(false);
        ui->doubleSpinBoxCursorSpline1X2->setEnabled(false);
        ui->doubleSpinBoxCursorSpline1Y->setEnabled(false);
        ui->doubleSpinBoxCursorSpline1Y2->setEnabled(false);

        ui->pushButtonLinkZone2->setCheckable(true);
        ui->pushButtonLinkZone2->setChecked(false);
        ui->pushButtonLinkZone2->setEnabled(false);

        ui->doubleSpinBoxCursorSpline2X->setEnabled(false);
        ui->doubleSpinBoxCursorSpline2X2->setEnabled(false);
        ui->doubleSpinBoxCursorSpline2Y->setEnabled(false);
        ui->doubleSpinBoxCursorSpline2Y2->setEnabled(false);

        ui->pushButtonDelBaseLine->setEnabled(false);

        ui->textEditPolyOrder->setEnabled(false);

        ui->comboBoxInterpolCurve->setEnabled(false);
    }
}

void FenEnfantGraph::cursorMangementInterpol(const int &state)
{
    if(state == 2)
    {
        //key control pressed
        controlPressed = false;

        ui->checkBoxCurseurNew->setChecked(false);

        QVector<double> dataInfo = mathMethod->dataInfo();
        //zone 1
        ui->doubleSpinBoxCursorSpline1X->setMinimum(dataInfo.at(0));
        ui->doubleSpinBoxCursorSpline1X->setMaximum(dataInfo.at(1));
        ui->doubleSpinBoxCursorSpline1X->setSingleStep(dataInfo.at(2));
        ui->doubleSpinBoxCursorSpline1X2->setMinimum(dataInfo.at(0));
        ui->doubleSpinBoxCursorSpline1X2->setMaximum(dataInfo.at(1));
        ui->doubleSpinBoxCursorSpline1X2->setSingleStep(dataInfo.at(2));

        ui->doubleSpinBoxCursorSpline1Y->setMinimum(dataInfo.at(5));
        ui->doubleSpinBoxCursorSpline1Y->setMaximum(dataInfo.at(6));
        ui->doubleSpinBoxCursorSpline1Y->setSingleStep(dataInfo.at(2));
        ui->doubleSpinBoxCursorSpline1Y2->setMinimum(dataInfo.at(5));
        ui->doubleSpinBoxCursorSpline1Y2->setMaximum(dataInfo.at(6));
        ui->doubleSpinBoxCursorSpline1Y2->setSingleStep(dataInfo.at(2));

        //zone 2
        ui->doubleSpinBoxCursorSpline2X->setMinimum(dataInfo.at(0));
        ui->doubleSpinBoxCursorSpline2X->setMaximum(dataInfo.at(1));
        ui->doubleSpinBoxCursorSpline2X->setSingleStep(dataInfo.at(2));
        ui->doubleSpinBoxCursorSpline2X2->setMinimum(dataInfo.at(0));
        ui->doubleSpinBoxCursorSpline2X2->setMaximum(dataInfo.at(1));
        ui->doubleSpinBoxCursorSpline2X2->setSingleStep(dataInfo.at(2));

        ui->doubleSpinBoxCursorSpline2Y->setMinimum(dataInfo.at(5));
        ui->doubleSpinBoxCursorSpline2Y->setMaximum(dataInfo.at(6));
        ui->doubleSpinBoxCursorSpline2Y->setSingleStep(dataInfo.at(2));
        ui->doubleSpinBoxCursorSpline2Y2->setMinimum(dataInfo.at(5));
        ui->doubleSpinBoxCursorSpline2Y2->setMaximum(dataInfo.at(6));
        ui->doubleSpinBoxCursorSpline2Y2->setSingleStep(dataInfo.at(2));

        createCursorInterpol();
    }
    else if (state == 0)
    {
        killCursorInterpol();
    }
}

void FenEnfantGraph::createCursorInterpol()
{    
    double viewLowV(ui->customPlot->yAxis->range().lower);
    double viewUpV(ui->customPlot->yAxis->range().upper);
    double viewCenterV ((int)(viewLowV + viewUpV)/2);

    double viewLeftH(ui->customPlot->xAxis->range().lower);
    double viewRightH(ui->customPlot->xAxis->range().upper);
    double viewCenterH ((int)(viewRightH + viewLeftH)/2);

    double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    double incrementH((mathMethod->dataInfo().at(1))/100);

    if(ui->checkBoxTimeAbsis->checkState() == 0)
    {
        //Zone 1
        ui->doubleSpinBoxCursorSpline1X->setValue( (viewCenterH/2 -incrementH));
        ui->doubleSpinBoxCursorSpline1X2->setValue( (viewCenterH/2 +incrementH));
        ui->doubleSpinBoxCursorSpline1Y->setValue( (viewCenterV -incrementV));
        ui->doubleSpinBoxCursorSpline1Y2->setValue( (viewCenterV +incrementV));
        //Zone 2
        ui->doubleSpinBoxCursorSpline2X->setValue( (viewCenterH * 1.5 -incrementH));
        ui->doubleSpinBoxCursorSpline2X2->setValue( (viewCenterH * 1.5 +incrementH));
        ui->doubleSpinBoxCursorSpline2Y->setValue( (viewCenterV -incrementV));
        ui->doubleSpinBoxCursorSpline2Y2->setValue( (viewCenterV +incrementV));
    }
    else if(ui->checkBoxTimeAbsis->checkState() == 2)
    {
        ui->doubleSpinBoxCursorSpline1Y->setValue( (viewCenterH/2 -incrementH) - offsetTime);
        ui->doubleSpinBoxCursorSpline1Y2->setValue( (viewCenterH/2 +incrementH) - offsetTime);

        ui->doubleSpinBoxCursorSpline2Y->setValue( (viewCenterH * 1.5 -incrementH) - offsetTime);
        ui->doubleSpinBoxCursorSpline2Y2->setValue( (viewCenterH * 1.5 +incrementH) - offsetTime);

        ui->doubleSpinBoxCursorSpline1X->setValue((viewCenterV -incrementV));
        ui->doubleSpinBoxCursorSpline1X2->setValue((viewCenterV +incrementV));

        ui->doubleSpinBoxCursorSpline2X->setValue( (viewCenterV -incrementV));
        ui->doubleSpinBoxCursorSpline2X2->setValue( (viewCenterV +incrementV));
    }
    //zone 1
    setCursorV("cursorInterpol 1Z1", (viewCenterH/2 -incrementH), penCursorInterpolZ1);
    setCursorV("cursorInterpol 2Z1", (viewCenterH/2 +incrementH), penCursorInterpolZ1);
    setCursorH("cursorInterpol 1Z1 H", (viewCenterV -incrementV), penCursorInterpolZ1);
    setCursorH("cursorInterpol 2Z1 H", (viewCenterV +incrementV), penCursorInterpolZ1);

    //zone 2
    setCursorV("cursorInterpol 1Z2", (viewCenterH * 1.5 -incrementH), penCursorInterpolZ2);
    setCursorV("cursorInterpol 2Z2", (viewCenterH * 1.5 +incrementH), penCursorInterpolZ2);
    setCursorH("cursorInterpol 1Z2 H", (viewCenterV -incrementV), penCursorInterpolZ2);
    setCursorH("cursorInterpol 2Z2 H", (viewCenterV +incrementV), penCursorInterpolZ2);

    createCursorInterpolConnection();
}

void FenEnfantGraph::setCursorV(const QString &cursorName, const double &posCursor, const QPen &pen, const double &offset)
{
    double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    QVector<double> absTab;
    for(int i(0); i <= 1; i++)
    {
        absTab.push_back(posCursor + offset);
    }

    QVector<double> ordTab;

    if(cursorName == "cursorInterpol 1Z1")
    {
        ordTab.push_back(ui->doubleSpinBoxCursorSpline1Y->value() - incrementV);
        ordTab.push_back(ui->doubleSpinBoxCursorSpline1Y2->value() + incrementV);
    }
    else if(cursorName == "cursorInterpol 2Z1")
    {
        ordTab.push_back(ui->doubleSpinBoxCursorSpline1Y->value() - incrementV);
        ordTab.push_back(ui->doubleSpinBoxCursorSpline1Y2->value() + incrementV);
    }
    else if(cursorName == "cursorInterpol 1Z2")
    {
        ordTab.push_back(ui->doubleSpinBoxCursorSpline2Y->value() - incrementV);
        ordTab.push_back(ui->doubleSpinBoxCursorSpline2Y2->value() + incrementV);
    }
    else if(cursorName == "cursorInterpol 2Z2")
    {
        ordTab.push_back(ui->doubleSpinBoxCursorSpline2Y->value() - incrementV);
        ordTab.push_back(ui->doubleSpinBoxCursorSpline2Y2->value() + incrementV);
    }
    else
    {
        ordTab.push_back(ui->customPlot->yAxis->range().lower);
        ordTab.push_back(ui->customPlot->yAxis->range().upper);
    }

    //Draw cursor
    displayCursor(cursorName, absTab, ordTab, pen);
}

void FenEnfantGraph::createCursorInterpolConnection()
{
    connect(signalMapperCursorV, SIGNAL(mapped(int)), this, SLOT(moveCursorV(int)));
    connect(signalMapperCursorV, SIGNAL(mapped(int)), this, SLOT(resizeCursorSpinboxInterpolH(int)));

    connect(signalMapperCursorH, SIGNAL(mapped(int)), this, SLOT(moveCursorH(int)));
    connect(signalMapperCursorH, SIGNAL(mapped(int)), this, SLOT(resizeCursorSpinboxInterpolV(int)));
}

void FenEnfantGraph::killCursorInterpol()
{
    ui->customPlot->removeGraph(indexGraph["cursorInterpol 2Z2 H"]);
    ui->customPlot->removeGraph(indexGraph["cursorInterpol 1Z2 H"]);
    ui->customPlot->removeGraph(indexGraph["cursorInterpol 2Z2"]);
    ui->customPlot->removeGraph(indexGraph["cursorInterpol 1Z2"]);

    ui->customPlot->removeGraph(indexGraph["cursorInterpol 2Z1 H"]);
    ui->customPlot->removeGraph(indexGraph["cursorInterpol 1Z1 H"]);
    ui->customPlot->removeGraph(indexGraph["cursorInterpol 2Z1"]);
    ui->customPlot->removeGraph(indexGraph["cursorInterpol 1Z1"]);
    ui->customPlot->replot();

    //erase cursor from indexGraph
    eraseGraphNameFromIndex("cursorInterpol 1Z1");
    eraseGraphNameFromIndex("cursorInterpol 2Z1");
    eraseGraphNameFromIndex("cursorInterpol 1Z2");
    eraseGraphNameFromIndex("cursorInterpol 2Z2");

    eraseGraphNameFromIndex("cursorInterpol 1Z1 H");
    eraseGraphNameFromIndex("cursorInterpol 2Z1 H");
    eraseGraphNameFromIndex("cursorInterpol 1Z2 H");
    eraseGraphNameFromIndex("cursorInterpol 2Z2 H");

    killCursorInterpolConnection();
}

void FenEnfantGraph::killCursorInterpolConnection()
{
    disconnect(signalMapperCursorV, SIGNAL(mapped(int)), this, SLOT(moveCursorV(int)));
    disconnect(signalMapperCursorV, SIGNAL(mapped(int)), this, SLOT(resizeCursorSpinboxInterpolH(int)));

    disconnect(signalMapperCursorH, SIGNAL(mapped(int)), this, SLOT(moveCursorH(int)));
    disconnect(signalMapperCursorH, SIGNAL(mapped(int)), this, SLOT(resizeCursorSpinboxInterpolV(int)));
}

void FenEnfantGraph::resizeCursorSpinboxInterpolV(const int &id)
{
    Q_UNUSED(id)

    sizeCursorV("cursorInterpol 1Z1", ui->doubleSpinBoxCursorSpline1X);
    sizeCursorV("cursorInterpol 2Z1", ui->doubleSpinBoxCursorSpline1X2);

    sizeCursorV("cursorInterpol 1Z2", ui->doubleSpinBoxCursorSpline2X);
    sizeCursorV("cursorInterpol 2Z2", ui->doubleSpinBoxCursorSpline2X2);
}

    //Create horisontal cursor
void FenEnfantGraph::setCursorH(const QString &cursorName, const double &posCursor, const QPen &pen)
{
    double incrementH((mathMethod->dataInfo().at(1))/100);
    QVector<double> absTab;

    if(cursorName == "cursorInterpol 1Z1 H")
    {
        absTab.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH);
        absTab.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH);
    }
    else if(cursorName == "cursorInterpol 2Z1 H")
    {
        absTab.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH);
        absTab.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH);
    }
    else if(cursorName == "cursorInterpol 1Z2 H")
    {
        absTab.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH);
        absTab.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH);
    }
    else if(cursorName == "cursorInterpol 2Z2 H")
    {
        absTab.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH);
        absTab.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH);
    }
    else
    {
        absTab.push_back(ui->customPlot->xAxis->range().lower);
        absTab.push_back(ui->customPlot->xAxis->range().upper);
    }

    QVector<double> ordTab;
    for(int i(0); i <= 1; i++)
    {
        ordTab.push_back(posCursor);
    }

    //Draw cursor
    displayCursor(cursorName, absTab, ordTab, pen);
}

void FenEnfantGraph::moveCursorH(const int &id)
{
    QString name;
    double value;
    if (id == 0)
    {
         name = "cursorInterpol 1Z1 H";
         value = ui->doubleSpinBoxCursorSpline1Y->value();
    }
    else if (id == 1)
    {
         name = "cursorInterpol 2Z1 H";
         value = ui->doubleSpinBoxCursorSpline1Y2->value();
    }
    else if (id == 2)
    {
         name = "cursorInterpol 1Z2 H";
         value = ui->doubleSpinBoxCursorSpline2Y->value();
    }
    else if (id == 3)
    {
         name = "cursorInterpol 2Z2 H";
         value = ui->doubleSpinBoxCursorSpline2Y2->value();
    }

    //Change value of keys
    QList<double> keysList(ui->customPlot->graph(indexGraph[name])->data()->keys());
    QVector<double> keysVector;

    /*for (int i(0); i <= keysList.size()-1; i++)
    {
        keysVector.push_back(keysList.at(i));
    }*/

    if(ui->checkBoxTimeAbsis->checkState() == 0)
    {
        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(keysList.at(i));
        }
    }
    else if (ui->checkBoxTimeAbsis->checkState() == 2)
    {
        for (int i(0); i <= keysList.size()-1; i++)
        {
            keysVector.push_back(keysList.at(i) + offsetTime);
        }
    }


    // extract values from QCPData and create a QVector of values
    QList<QCPData> valuesList(ui->customPlot->graph(indexGraph[name])->data()->values());
    QVector<double> valuesVector;

    for (int i(0); i <= valuesList.size()-1; i++)
    {
        valuesVector.push_back(value);
    }

    ui->customPlot->graph(indexGraph[name])->setData(keysVector, valuesVector);
    ui->customPlot->replot();
}

void FenEnfantGraph::resizeCursorSpinboxInterpolH(const int &id)
{
    Q_UNUSED(id)

    sizeCursorH("cursorInterpol 1Z1 H", ui->doubleSpinBoxCursorSpline1Y);
    sizeCursorH("cursorInterpol 2Z1 H", ui->doubleSpinBoxCursorSpline1Y2);

    sizeCursorH("cursorInterpol 1Z2 H", ui->doubleSpinBoxCursorSpline2Y);
    sizeCursorH("cursorInterpol 2Z2 H", ui->doubleSpinBoxCursorSpline2Y2);
}

void FenEnfantGraph::sizeCursorH(const QString &name, QDoubleSpinBox *spinbox)
{
    double incrementH((mathMethod->dataInfo().at(1))/100);
    double value(0);
    value = spinbox->value();

    //Change value of keys
    QVector<double> keysVector;

    if(name == "cursorInterpol 1Z1 H")
    {
        if(ui->checkBoxTimeAbsis->checkState() == 0)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH);
        }
        else if (ui->checkBoxTimeAbsis->checkState() == 2)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH + offsetTime);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH + offsetTime);
        }
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH);
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH);
    }
    else if(name == "cursorInterpol 2Z1 H")
    {
        if(ui->checkBoxTimeAbsis->checkState() == 0)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH);
        }
        else if (ui->checkBoxTimeAbsis->checkState() == 2)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH +offsetTime);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH +offsetTime);
        }
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline1X->value() - incrementH);
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline1X2->value() + incrementH);
    }
    else if(name == "cursorInterpol 1Z2 H")
    {
        if(ui->checkBoxTimeAbsis->checkState() == 0)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH);
        }
        else if (ui->checkBoxTimeAbsis->checkState() == 2)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH +offsetTime);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH +offsetTime);
        }
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH);
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH);
    }
    else if(name == "cursorInterpol 2Z2 H")
    {
        if(ui->checkBoxTimeAbsis->checkState() == 0)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH);
        }
        else if (ui->checkBoxTimeAbsis->checkState() == 2)
        {
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH +offsetTime);
            keysVector.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH +offsetTime);
        }
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline2X->value() - incrementH);
        //keysVector.push_back(ui->doubleSpinBoxCursorSpline2X2->value() + incrementH);
    }

    // extract values from QCPData and create a QVector of values
    QList<QCPData> valuesList(ui->customPlot->graph(indexGraph[name])->data()->values());
    QVector<double> valuesVector;

    for (int i(0); i <= valuesList.size()-1; i++)
    {
        valuesVector.push_back(value);
    }

    ui->customPlot->graph(indexGraph[name])->setData(keysVector, valuesVector);
    ui->customPlot->replot();
}
    //Create horisontal cursor end
    //link cursor Zone 1
void FenEnfantGraph::linkCursorZ1(const bool &state)
{
    if (state == true)
    {
        connect(ui->doubleSpinBoxCursorSpline1X, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z1X(double)));
        connect(ui->doubleSpinBoxCursorSpline1X2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z1X(double)));
        connect(ui->doubleSpinBoxCursorSpline1Y, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z1Y(double)));
        connect(ui->doubleSpinBoxCursorSpline1Y2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z1Y(double)));
    }

    else if (state == false)
    {
        disconnect(ui->doubleSpinBoxCursorSpline1X, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z1X(double)));
        disconnect(ui->doubleSpinBoxCursorSpline1X2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z1X(double)));
        disconnect(ui->doubleSpinBoxCursorSpline1Y, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z1Y(double)));
        disconnect(ui->doubleSpinBoxCursorSpline1Y2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z1Y(double)));
    }
}

void FenEnfantGraph::linkCursor1Z1X(const double &value)
{
    double incrementH((mathMethod->dataInfo().at(1))/100);

    if( ui->pushButtonLinkZone1->isChecked() == true && ui->doubleSpinBoxCursorSpline1X->value() != ui->doubleSpinBoxCursorSpline1X2->value() - 1)
    {
        ui->doubleSpinBoxCursorSpline1X2->setValue(value +incrementH);
    }
}

void FenEnfantGraph::linkCursor2Z1X(const double &value)
{
    double incrementH((mathMethod->dataInfo().at(1))/100);

    if( ui->pushButtonLinkZone1->isChecked() == true && ui->doubleSpinBoxCursorSpline1X2->value() != ui->doubleSpinBoxCursorSpline1X->value() + 1)
    {
        ui->doubleSpinBoxCursorSpline1X->setValue(value -incrementH);
    }
}

void FenEnfantGraph::linkCursor1Z1Y(const double &value)
{
    double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    if( ui->pushButtonLinkZone1->isChecked() == true && ui->doubleSpinBoxCursorSpline1Y->value() != ui->doubleSpinBoxCursorSpline1Y2->value() - 1)
    {
        ui->doubleSpinBoxCursorSpline1Y2->setValue(value +incrementV);
    }
}

void FenEnfantGraph::linkCursor2Z1Y(const double &value)
{
    double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    if( ui->pushButtonLinkZone1->isChecked() == true && ui->doubleSpinBoxCursorSpline1Y2->value() != ui->doubleSpinBoxCursorSpline1Y->value() + 1)
    {
        ui->doubleSpinBoxCursorSpline1Y->setValue(value -incrementV);
    }
}
    //link cursor Zone 2
void FenEnfantGraph::linkCursorZ2(const bool &state)
{
    if (state == true)
    {
        connect(ui->doubleSpinBoxCursorSpline2X, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z2X(double)));
        connect(ui->doubleSpinBoxCursorSpline2X2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z2X(double)));
        connect(ui->doubleSpinBoxCursorSpline2Y, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z2Y(double)));
        connect(ui->doubleSpinBoxCursorSpline2Y2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z2Y(double)));
    }

    else if (state == false)
    {
        disconnect(ui->doubleSpinBoxCursorSpline2X, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z2X(double)));
        disconnect(ui->doubleSpinBoxCursorSpline2X2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z2X(double)));
        disconnect(ui->doubleSpinBoxCursorSpline2Y, SIGNAL(valueChanged(double)), this, SLOT(linkCursor1Z2Y(double)));
        disconnect(ui->doubleSpinBoxCursorSpline2Y2, SIGNAL(valueChanged(double)), this, SLOT(linkCursor2Z2Y(double)));
    }
}

void FenEnfantGraph::linkCursor1Z2X(const double &value)
{
    double incrementH((mathMethod->dataInfo().at(1))/100);
    if( ui->pushButtonLinkZone2->isChecked() == true && ui->doubleSpinBoxCursorSpline2X->value() != ui->doubleSpinBoxCursorSpline2X2->value() - 1)
    {
        ui->doubleSpinBoxCursorSpline2X2->setValue(value +incrementH);
    }
}

void FenEnfantGraph::linkCursor2Z2X(const double &value)
{
    double incrementH((mathMethod->dataInfo().at(1))/100);
    if( ui->pushButtonLinkZone1->isChecked() == true && ui->doubleSpinBoxCursorSpline2X2->value() != ui->doubleSpinBoxCursorSpline2X->value() + 1)
    {
        ui->doubleSpinBoxCursorSpline2X->setValue(value -incrementH);
    }
}

void FenEnfantGraph::linkCursor1Z2Y(const double &value)
{
    double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    if( ui->pushButtonLinkZone2->isChecked() == true && ui->doubleSpinBoxCursorSpline2Y->value() != ui->doubleSpinBoxCursorSpline2Y2->value() - 1)
    {
        ui->doubleSpinBoxCursorSpline2Y2->setValue(value +incrementV);
    }
}

void FenEnfantGraph::linkCursor2Z2Y(const double &value)
{
    double incrementV((mathMethod->dataInfo().at(6)-mathMethod->dataInfo().at(5))/100);
    if( ui->pushButtonLinkZone2->isChecked() == true && ui->doubleSpinBoxCursorSpline2Y2->value() != ui->doubleSpinBoxCursorSpline2Y->value() + 1)
    {
        ui->doubleSpinBoxCursorSpline2Y->setValue(value -incrementV);
    }
}

//Generate interpolation


void FenEnfantGraph::addItemToComboboxInterpol()
{
    for(std::map<QString,int>::iterator it(indexGraph.begin()); it!= indexGraph.end(); it++)
    {
        QString nameCurve(it->first);
        if (nameCurve != "cursorNew 1" || nameCurve != "cursorNew 2" || nameCurve != "cursorInterpol 1Z1" || nameCurve != "cursorInterpol 2Z1" || nameCurve != "cursorInterpol 1Z2" || nameCurve != "cursorInterpol 2Z2" || nameCurve != "cursorInterpol 1Z1 H" || nameCurve != "cursorInterpol 2Z1 H" || nameCurve != "cursorInterpol 1Z2 H" || nameCurve != "cursorInterpol 2Z2 H")
        {
            ui->comboBoxInterpolCurve->addItem(nameCurve);
        }
    }
}

void FenEnfantGraph::destroyItemFromCombobox()
{
    ui->comboBoxInterpolCurve->clear();
}

void FenEnfantGraph::delBaseLine()
{
    QVector<double> dataReturn;
    double valueZ1Left(ui->doubleSpinBoxCursorSpline1X->value());
    double valueZ1Right(ui->doubleSpinBoxCursorSpline1X2->value());
    double valueZ1Up(ui->doubleSpinBoxCursorSpline1Y2->value());
    double valueZ1Down(ui->doubleSpinBoxCursorSpline1Y->value());

    double valueZ2Left(ui->doubleSpinBoxCursorSpline2X->value());
    double valueZ2Right(ui->doubleSpinBoxCursorSpline2X2->value());
    double valueZ2Up(ui->doubleSpinBoxCursorSpline2Y2->value());
    double valueZ2Down(ui->doubleSpinBoxCursorSpline2Y->value());

    dataReturn.push_back(valueZ1Left);  //0
    dataReturn.push_back(valueZ1Right); //1
    dataReturn.push_back(valueZ1Up);    //2
    dataReturn.push_back(valueZ1Down);  //3

    dataReturn.push_back(valueZ2Left);  //4
    dataReturn.push_back(valueZ2Right); //5
    dataReturn.push_back(valueZ2Up);    //6
    dataReturn.push_back(valueZ2Down);  //7

    mathMethod->setPolyOrder(3);
    QVector<QVector<double> > tabReg;
    tabReg = mathMethod->generatePoint(indexGraph[ui->comboBoxInterpolCurve->currentText()], dataReturn);

    QVector<QVector<double> > tabDelBaseLine;
    tabDelBaseLine = mathMethod->delBaseLine( indexGraph[ui->comboBoxInterpolCurve->currentText()], tabReg);
    displayMathFunctionCurve(tabDelBaseLine);

    //Get regression factor
    QVector<double> regFactor(mathMethod->getRegFactor());

    //Get corelation coefficient
    double cc(mathMethod->getRegCC());

    //Get baricenter value
    double baricenter(mathMethod->baricenterCurve(tabDelBaseLine));

    //Get text doc text
    QString textDoc(ui->textEditPolyOrder->toPlainText());
    QString text(textDoc);

    QString textCurve;
    QString textCC;
    QString textBar;
    QString textPoly;
    QString textSpacer;

    if (regFactor.size() == 2)
    {
        textCurve = tr("Curve: %1 \n").arg(ui->comboBoxInterpolCurve->currentText());
        textCC = tr("R^2 = %1 \n").arg(cc);
        textBar = tr("Bar = %1 \n").arg(baricenter);
        textSpacer = tr("\n\n");
        textPoly = tr("f(x) = %1x + %2 \n").arg(regFactor.at(1))
                                            .arg(regFactor.at(0));
        text += textCurve + textPoly + textCC + textBar + textSpacer;
        ui->textEditPolyOrder->setText(text);
    }
    else if (regFactor.size() == 3)
    {
        textCurve = tr("Curve: %1 \n").arg(ui->comboBoxInterpolCurve->currentText());
        textCC = tr("R^2 = %1 \n").arg(cc);
        textBar = tr("Bar = %1 \n").arg(baricenter);
        textSpacer = tr("\n\n");
        textPoly = tr("f(x) = %1x^2 + %2x + %3\n").arg(regFactor.at(2))
                                                    .arg(regFactor.at(1))
                                                    .arg(regFactor.at(0));
        text += textCurve + textPoly + textCC + textBar + textSpacer;
        ui->textEditPolyOrder->setText(text);
    }
    else if (regFactor.size() == 4)
    {
        textCurve = tr("Curve: %1 \n").arg(ui->comboBoxInterpolCurve->currentText());
        textCC = tr("R^2 = %1 \n").arg(cc);
        textBar = tr("Bar = %1 \n").arg(baricenter);
        textSpacer = tr("\n\n");
        textPoly = tr("f(x) = %1x^3 + %2x^2 + %3x + %4\n").arg(regFactor.at(3))
                                                            .arg(regFactor.at(2))
                                                            .arg(regFactor.at(1))
                                                            .arg(regFactor.at(0));
        text += textCurve + textPoly + textCC + textBar + textSpacer;
        ui->textEditPolyOrder->setText(text);
    }
    else if (regFactor.size() == 5)
    {
        textCurve = tr("Curve: %1 \n").arg(ui->comboBoxInterpolCurve->currentText());
        textCC = tr("R^2 = %1 \n").arg(cc);
        textBar = tr("Bar = %1 \n").arg(baricenter);
        textSpacer = tr("\n\n");
        textPoly = textPoly = tr("f(x) = %1x^4 + %2x^3 + %3x^2 + %4x + %5\n").arg(regFactor.at(4))
                                                                                .arg(regFactor.at(3))
                                                                                .arg(regFactor.at(2))
                                                                                .arg(regFactor.at(1))
                                                                                .arg(regFactor.at(0));
        text += textCurve + textPoly + textCC + textBar + textSpacer;
        ui->textEditPolyOrder->setText(text);
    }
    else if (regFactor.size() == 6)
    {
        textCurve = tr("Curve: %1 \n").arg(ui->comboBoxInterpolCurve->currentText());
        textCC = tr("R^2 = %1 \n").arg(cc);
        textBar = tr("Bar = %1 \n").arg(baricenter);
        textSpacer = tr("\n\n");
        textPoly = textPoly = tr("f(x) = %1x^5 + %2x^4 + %3x^3 + %4x^2 + %5x + %6\n").arg(regFactor.at(5))
                                                                                        .arg(regFactor.at(4))
                                                                                        .arg(regFactor.at(3))
                                                                                        .arg(regFactor.at(2))
                                                                                        .arg(regFactor.at(1))
                                                                                        .arg(regFactor.at(0));
        text += textCurve + textPoly + textCC + textBar + textSpacer;
        ui->textEditPolyOrder->setText(text);
    }
    else if (regFactor.size() == 7)
    {
        textCurve = tr("Curve: %1 \n").arg(ui->comboBoxInterpolCurve->currentText());
        textCC = tr("R^2 = %1 \n").arg(cc);
        textBar = tr("Bar = %1 \n").arg(baricenter);
        textSpacer = tr("\n\n");
        textPoly = textPoly = tr("f(x) = %1x^6 + %2x^5 + %3x^4 + %4x^3 + %5x^2 + %6x + %7\n").arg(regFactor.at(6))
                                                                                                .arg(regFactor.at(5))
                                                                                                .arg(regFactor.at(4))
                                                                                                .arg(regFactor.at(3))
                                                                                                .arg(regFactor.at(2))
                                                                                                .arg(regFactor.at(1))
                                                                                                .arg(regFactor.at(0));
        text += textCurve + textPoly + textCC + textBar + textSpacer;
        ui->textEditPolyOrder->setText(text);
    }
}
//Del base line interaction end
