#ifndef FENENFANTGRAPH_H
#define FENENFANTGRAPH_H

#include <QWidget>
#include <qcustomplot.h>
#include <map>
#include <QTimer>

class MathFunction;

namespace Ui {
class FenEnfantGraph;
}

class FenEnfantGraph : public QWidget
{
    Q_OBJECT

public:
    explicit FenEnfantGraph(QWidget *parent = 0);
    ~FenEnfantGraph();

    bool LoadTabData(const QStringList &fileInfo, const QStringList &header, const QVector<QVector<double> > &tab);
    QString userFriendlyCurrentFile();
    void averageCurve(const int &nbTab, const int &sampleTime, const QVector<QVector<double> > &tab);
    void middleCurve(const int &nbTab, const int &sampleTime,const QVector<QVector<double> > &tab);
    QString currentFile() { return curFile; }

public slots:
    void cursor1(const double &posCursor);
    void cursor2(const double &posCursor);
    void cursorHeightScroll(QWheelEvent *event);
    void cursorHeightMouved(QMouseEvent* event );
    void cursorHeightPressed(QMouseEvent* event);
    void cursorEnable(const int& state);
    void curveDisplay(const int &nbCurve);
    void zoom();
    void createCurve();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void changeSelectedGraphColor();
    void changeSelectedGraphThickness();
    void changeSelectedGraphScatter();
    void changeColorBGB();
    void changeColorBGW();
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title);
    //void selectionChanged();
    void exportGraphAsPng();
    void exportGraphAsPdf();
    void exportGraphAsJpeg();
    void linkPngValueX(const int &value);
    void linkJpegValueX(const int &value);
    void linkPngValueY(const int &value);
    void linkJpegValueY(const int &value);
    void setGraphAbsisTime();
    void changeAbsisName();
    //NewCursor
    void cursorMangement(const int &state);
    void moveCursor1(const int &value);
    void moveCursor2(const int &value);
    void resizeCursorScroll(const QWheelEvent* &event);
    void resizeCursorMove(const QMouseEvent* &event);
    void resizeCursorRelease(const QMouseEvent *&event);
    //NewCursor end




signals:
    void cursor1Update();
    void cursor2Update();

private:
    void setCurrentFile(const QString &fileName);
    void setInfoData(const QStringList &fileInfo);
    void setGraph(const QStringList &header, const QVector<QVector<double> > &tab);
    void setGraphParameter();
    void defineAxis(const QStringList &header);
    void setTabCurve(const QString &nameCurve);
    //Cursor
    void initCursor();
    void setCursorCurveV(const int cursorId, const double &posCursor, const int &nbGraph, const QCPRange &cursorHeight);
    void keyPressEvent(QKeyEvent *event);
    //Cursor end
    //new Cursor
    void createCursorNew();
    void setCursorVNew(const int cursorId, const double &posCursor);
    void displayCursor(const QString &name, const QVector<double> &absTab, const QVector<double> &ordTab);
    void killCursorNew();
    void eraseGraphNameFromIndex(const QString &name);
    void sizeCursor1();
    void sizeCursor2();
    //new cursor end

    void createCurveIntialisation();
    void displayMathFunctionCurve(const QVector<QVector<double> > &tab);
    QString curveName(const QVector<double> &tabId);
    void calculateOffsetTime(const QStringList &fileInfo);
    void addTimeOffsetToGraph();
    void subTimeOffsetToGraph();
    double timeUnit();

    MathFunction *mathMethod;
    QColor randomColor(const QString &colorType);
    QString strippedName(const QString &fullFileName);
    Ui::FenEnfantGraph *ui;
    std::map<QString,int>  indexGraph;
    QSignalMapper *signalMapper;
    QString curFile;
    QTimer dataTimer;
    bool blackTheme;
    QString graphTitle;
    double offsetTime;
    QStringList m_header;
};

#endif // FENENFANTGRAPH_H
