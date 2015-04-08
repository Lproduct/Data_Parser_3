#include "FenPrincipale.h"
#include "FenEnfantTab.h"
#include "FenEnfantGraph.h"
#include "DataParser.h"

FenPrincipale::FenPrincipale()
{
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);

    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(updateMenus()));

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    readSettings();

    setWindowTitle(tr("Data Parser"));
    setWindowIcon(QIcon(":/images/data_parser_icon.png"));
    setMinimumSize(500, 350);
    setUnifiedTitleAndToolBarOnMac(true);
}

void FenPrincipale::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow())
    {
        event->ignore();
    }
    else
    {
        writeSettings();
        event->accept();
    }
}

void FenPrincipale::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
    {
        //Check if the file is already open
        QMdiSubWindow *existingGraph = findMdiChildGraph(fileName);

        if (existingGraph)
        {
            mdiArea->setActiveSubWindow(existingGraph);
            return;
        }

        //Parsing data
        DataParser *parserData = new DataParser(fileName);
        QStringList infoFile(parserData->getInfoFile());
        QStringList header(parserData->getHeader());
        QVector<QVector<double> > tab(parserData->getTab());

        FenEnfantGraph *childGraph = createGraphMdiChild();
        if (childGraph->LoadTabData(infoFile, header, tab))
        {
            statusBar()->showMessage(tr("File loaded"), 2000);
            childGraph->show();
        }
        else
        {
            childGraph->close();
        }
    }
}


void FenPrincipale::about()
{
   QMessageBox::about(this, tr("About Data Parser"),
            tr("Data Parser is devellop by LProduct\n\nContact: Lvil.180@gmail.com"));
}

void FenPrincipale::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
    closeAllAct->setEnabled(hasMdiChild);
    tileAct->setEnabled(hasMdiChild);
    cascadeAct->setEnabled(hasMdiChild);
    nextAct->setEnabled(hasMdiChild);
    previousAct->setEnabled(hasMdiChild);
    separatorAct->setVisible(hasMdiChild);
}

void FenPrincipale::updateWindowMenu()
{
    windowMenu->clear();
    /*windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);*/

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); i++)
    {
        FenEnfantGraph *childGraph = qobject_cast<FenEnfantGraph *>(windows.at(i)->widget());

        QString text;
        if (i < 9)
        {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(childGraph->userFriendlyCurrentFile());
        }
        else
        {
            text = tr("%1 %2").arg(i + 1)
                              .arg(childGraph->userFriendlyCurrentFile());
        }

        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(childGraph == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));
    }
}

FenEnfantGraph *FenPrincipale::createGraphMdiChild()
{
    FenEnfantGraph *childGraph = new FenEnfantGraph;
    mdiArea->addSubWindow(childGraph);

    return childGraph;
}

void FenPrincipale::createActions()
{

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

//! [0]
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
//! [0]

    closeAct = new QAction(tr("Cl&ose"), this);
    closeAct->setStatusTip(tr("Close the active window"));
    connect(closeAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeActiveSubWindow()));

    closeAllAct = new QAction(tr("Close &All"), this);
    closeAllAct->setStatusTip(tr("Close all the windows"));
    connect(closeAllAct, SIGNAL(triggered()),
            mdiArea, SLOT(closeAllSubWindows()));

    tileAct = new QAction(tr("&Tile"), this);
    tileAct->setStatusTip(tr("Tile the windows"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    cascadeAct = new QAction(tr("&Cascade"), this);
    cascadeAct->setStatusTip(tr("Cascade the windows"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    nextAct = new QAction(tr("Ne&xt"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("Move the focus to the next window"));
    connect(nextAct, SIGNAL(triggered()),
            mdiArea, SLOT(activateNextSubWindow()));

    previousAct = new QAction(tr("Pre&vious"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("Move the focus to the previous "
                                 "window"));
    connect(previousAct, SIGNAL(triggered()),
            mdiArea, SLOT(activatePreviousSubWindow()));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void FenPrincipale::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(exitAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void FenPrincipale::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
}

void FenPrincipale::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void FenPrincipale::readSettings()
{
    QSettings settings("Trolltech", "MDI Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void FenPrincipale::writeSettings()
{
    QSettings settings("Lproduct", "Data_Parser");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

FenEnfantGraph *FenPrincipale::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<FenEnfantGraph *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *FenPrincipale::findMdiChildGraph(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();

    for (int i = 0; i < windows.size(); i++)
    {
        FenEnfantGraph *mdiChildGraph = qobject_cast<FenEnfantGraph *>(windows.at(i)->widget());
        if (mdiChildGraph->currentFile() == canonicalFilePath)
            return windows.at(i);
    }

    return 0;
}

void FenPrincipale::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

