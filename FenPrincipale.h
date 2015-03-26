#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QMainWindow>
#include <QtWidgets>


//class FenEnfant;
class FenEnfantTab;
class FenEnfantGraph;
QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
QT_END_NAMESPACE

class FenPrincipale : public QMainWindow
{
    Q_OBJECT

public:
    FenPrincipale();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    //void newFile();
    void open();
    //void save();
    //void saveAs();
    //void cut();
    //void copy();
    //void paste();
    void about();
    void updateMenus();
    void updateWindowMenu();
    FenEnfantTab *createTabMdiChild();
    FenEnfantGraph *createGraphMdiChild();

    //void switchLayoutDirection();
    void setActiveSubWindow(QWidget *window);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    FenEnfantTab *activeMdiChild();
    FenEnfantGraph *activeMdiChildGraph();
    QMdiSubWindow *findMdiChildTab(const QString &fileName);
    QMdiSubWindow *findMdiChildGraph(const QString &fileName);

    //data parser function
    int findWordInTab(const QStringList& tab, const QString& word);
    QString fileTxtInfo(const QString& info);
    //data parser function

    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;
    QMenu *fileMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QAction *openAct;
    QAction *exitAct;
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *separatorAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

};

#endif // FENPRINCIPALE_H
