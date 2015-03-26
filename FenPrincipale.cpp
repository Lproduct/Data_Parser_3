#include "FenPrincipale.h"
#include "FenEnfant.h"
#include "FenEnfantTab.h"
#include "FenEnfantGraph.h"

FenPrincipale::FenPrincipale()
{
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateMenus()));
    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(setActiveSubWindow(QWidget*)));

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
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
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
        QMdiSubWindow *existingTab = findMdiChildTab(fileName);
        QMdiSubWindow *existingGraph = findMdiChildGraph(fileName);

        if (existingTab)
        {
            mdiArea->setActiveSubWindow(existingTab);
            mdiArea->setActiveSubWindow(existingGraph);
            return;
        }
        /*****************************************
         * Data parsing
         * **************************************/
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            QMessageBox::warning(this, tr("Data Parser"), tr("Cannot read file %1:\n%2").arg(fileName).arg(file.errorString()));
            //return false;
        }

        //Read all the data from the file
        QTextStream in(&file);
        QString dataTxt(in.readAll());

        // Read the whole txt file and create a list which contain every sentences
        QStringList tabData(dataTxt.split(QRegExp("[\r\n]"), QString::SkipEmptyParts));

        //file close
        file.close();

        // Looking for word position
        QString wordDate("Date :");
        QString wordHeure("Heure :");
        QString wordNomOperateur("Nom Opérateur :");
        QString wordNomSite("Nom du Site :");
        QString wordNomEssai("Nom de l'essai :");
        QString wordCommentaires("Commentaires :");

        //int fichierPosList = findWordInTab(tabData, wordFichier);
        int datePosList = findWordInTab(tabData, wordDate);
        int heurePosList = findWordInTab(tabData, wordHeure);
        int nomOperateurPosList = findWordInTab(tabData, wordNomOperateur);
        int sitePosList = findWordInTab(tabData, wordNomSite);
        int essaiPosList = findWordInTab(tabData, wordNomEssai);
        int commentairesPosList = findWordInTab(tabData, wordCommentaires);

        QString fichier(tabData.at(0));
        QString date(fileTxtInfo(tabData.at(datePosList)));
        QString heure(fileTxtInfo(tabData.at(heurePosList)));
        QString nomOperateur(fileTxtInfo(tabData.at(nomOperateurPosList)));
        QString nomSite(fileTxtInfo(tabData.at(sitePosList)));
        QString nomEssai(fileTxtInfo(tabData.at(essaiPosList)));
        QString commentaires(fileTxtInfo(tabData.at(commentairesPosList)));

        QStringList infoFile;
        infoFile.append(fileName);
        infoFile.append(fichier);
        infoFile.append(date);
        infoFile.append(heure);
        infoFile.append(nomOperateur);
        infoFile.append(nomSite);
        infoFile.append(nomEssai);
        infoFile.append(commentaires);

        //looking for header(curve name)
        QString wordTemps("Temps");
        int tempsPosList = findWordInTab(tabData, wordTemps);
        QString cell(tabData.at(tempsPosList));
        QStringList header(cell.split(QRegExp("\t")));

        //transform tab data into numeric tab. Two fisrt row represent the number of column and number
        //of row the other data organised as tab witch are stacked.
        QVector<double> tabNumericData(0);
        double numericData(0);
        QString cellStrData;
        QStringList cellStrDataList;

        int nbColumn(header.size());
        int nbRow(tabData.size() - tempsPosList -2);
        tabNumericData.push_back(nbColumn);
        tabNumericData.push_back(nbRow);

        //creat a tab witch contain all numeric tab data stacked
        for (int k(0); k<=header.size()-1; k++)
        {
            for ( int i(tempsPosList+1); i<=tabData.size()-1; i++)
            {
                cellStrData = tabData.at(i);
                cellStrDataList = cellStrData.split(QRegExp("\t"));
                numericData = cellStrDataList[k].toDouble();
                tabNumericData.push_back(numericData);
            }
        }

        /*****************************************
         * Data parsing end
         * **************************************/

        FenEnfantTab *childTab = new FenEnfantTab;
        mdiArea->addSubWindow(childTab);
        if (childTab->loadTabData(infoFile, header, tabNumericData))
        {
            statusBar()->showMessage(tr("File loaded"), 2000);
            childTab->show();
        }
        else
        {
            childTab->close();
        }

        FenEnfantGraph *childGraph = new FenEnfantGraph;
        mdiArea->addSubWindow(childGraph);
        if (childGraph->LoadTabData(tabNumericData, fileName))
        {
            statusBar()->showMessage(tr("File loaded"), 2000);
            childGraph->show();
        }
        else
        {
            childTab->close();
        }
    }
}
/**********************************
 * Data parser function
 * *******************************/
int FenPrincipale::findWordInTab(const QStringList& tab, const QString& word)
{
    // Return the position of a word in a QStringList
    bool wordIsFind(false);
    int wordPosLine = 0;
    while (wordIsFind == false)
    {
        QString cellControl(tab.at(wordPosLine));
        QStringList CellControlList(cellControl.split(QRegExp("\t")));
        wordIsFind = CellControlList.contains(word);
        wordPosLine++;
    }
    return wordPosLine-1;
}

QString FenPrincipale::fileTxtInfo(const QString& info)
{
    //Permit to parse information of the info file header, if there is no information the string is null
    QStringList infoList = info.split(QRegExp("\t"));
    if (infoList.size() == 1)
    {
        QString infoTr(" ");
        return infoTr;
    }
    else
    {
        QString infoTr2(infoList.at(1));
        return infoTr2;
    }
}
/***********************************
 * Data parser function end
 * ********************************/

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

    for (int i = 0; i < windows.size(); i+=2)
    {
        FenEnfantTab *childTab = qobject_cast<FenEnfantTab *>(windows.at(i)->widget());

        QString text;
        if (i < 9)
        {
            text = tr("&%1 %2").arg(i/2 + 1)
                               .arg(childTab->userFriendlyCurrentFile());
        }
        else
        {
            text = tr("%1 %2").arg(i/2 + 1)
                              .arg(childTab->userFriendlyCurrentFile());
        }

        QAction *action  = windowMenu->addAction(text);
        action->setCheckable(true);
        action ->setChecked(childTab == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windows.at(i));

        FenEnfantGraph *childGraph = qobject_cast<FenEnfantGraph *>(windows.at(i+1)->widget());
        QString text2;
        if (i < 9)
        {
            text2 = tr("   &%1").arg(childGraph->userFriendlyCurrentFile());//.arg(i/2 + 1)
        }
        else
        {
            text2 = tr("   %1").arg(childGraph->userFriendlyCurrentFile());//.arg(i/2 + 1)
        }

        QAction *action2  = windowMenu->addAction(text2);
        action->setCheckable(true);
        action ->setChecked(childGraph == activeMdiChildGraph());
        connect(action2, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action2, windows.at(i+1));
    }
}

FenEnfant *FenPrincipale::createMdiChild()
{
    FenEnfant *child = new FenEnfant;
    mdiArea->addSubWindow(child);

    return child;
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
    QSettings settings("Trolltech", "MDI Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

FenEnfantTab *FenPrincipale::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<FenEnfantTab *>(activeSubWindow->widget());        
    return 0;
}

FenEnfantGraph *FenPrincipale::activeMdiChildGraph()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<FenEnfantGraph *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *FenPrincipale::findMdiChildTab(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();

    for (int i = 0; i < windows.size(); i+=2)
    {
        FenEnfantTab *mdiChildTab = qobject_cast<FenEnfantTab *>(windows.at(i)->widget());
        if (mdiChildTab->currentFile() == canonicalFilePath)
            return windows.at(i);
    }

    return 0;
}

QMdiSubWindow *FenPrincipale::findMdiChildGraph(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();

    for (int i = 0; i < windows.size(); i+=2)
    {
        FenEnfantGraph *mdiChildGraph = qobject_cast<FenEnfantGraph *>(windows.at(i+1)->widget());
        if (mdiChildGraph->currentFile() == canonicalFilePath)
            return windows.at(i+1);
    }

    return 0;
}

void FenPrincipale::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

