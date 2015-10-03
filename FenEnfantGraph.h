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
    QString currentFile() { return curFile; }

public slots:
    //Tab curve
    void curveDisplay(const int &nbCurve);
    void delCurve(const int &nbCurve);
    void resizeGraphCurve(const int &nbCurve);
    void clearCustom(const int &nbCurve);
    //Zoom management
    void zoom();
    //Math curve
    void createCurveNew();
    void comboboxCurveNameInteraction(const int &nbGraph);
    //graph skin
    void contextMenuRequest(QPoint pos);    
    void moveLegend();
    void changeSelectedGraphColor();
    void changeSelectedGraphThickness();
    void changeSelectedGraphScatter();
    void changeSelectedGraphStyle();
    void changeSelectedGraphLineStyle();
    void changeColorBGB();
    void changeColorBGW();
    void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);
    void titleDoubleClick(QMouseEvent* event, QCPPlotTitle* title);
    //Export graph
    void exportGraphAsPng();
    void exportGraphAsPdf();
    void exportGraphAsJpeg();
    void exportGraphData();
    void linkPngValueX(const int &value);
    void linkJpegValueX(const int &value);
    void linkPngValueY(const int &value);
    void linkJpegValueY(const int &value);
    //Time absis
    void setGraphAbsisTime();
    void changeAbsisName();
    //NewCursor
    void cursorMangement(const int &state);
    void moveCursorV(const int &id);
    void resizeCursorScroll(const QWheelEvent* &event);
    void resizeCursorMouse(const QMouseEvent* &event);
    //NewCursor end
    void drawBetweenCursorState();
    //Custom Curve
    void customCurveManagement();
    void customCurve();
    //Cursor spinbox interaction
    void cursorSpinBoxInt(const int &ind);
    //Interpolation interaction
    void interpolationInteraction(const int &state);
    void cursorMangementInterpol(const int &state);
    void moveCursorH(const int &id);
    void resizeCursorSpinboxInterpolH(const int &id);
    void resizeCursorSpinboxInterpolV(const int &id);
    void sizeCursorV(const QString &name, QDoubleSpinBox *spinbox);
    void linkCursorZ1(const bool &state);
    void linkCursor1Z1X(const double &value);
    void linkCursor2Z1X(const double &value);
    void linkCursor1Z1Y(const double &value);
    void linkCursor2Z1Y(const double &value);
    void linkCursorZ2(const bool &state);
    void linkCursor1Z2X(const double &value);
    void linkCursor2Z2X(const double &value);
    void linkCursor1Z2Y(const double &value);
    void linkCursor2Z2Y(const double &value);
    void delBaseLine();
    //tab Custom
    void graphTabManagement();
    void graphTabShow();
    void graphTabActualisation(QVector<QVector<double> > data);
    //decay compensation
    void decayCompensationInteractionmanagement();
    void decayCompensation();
    //operation on graph
    void operationGraph();
    void clearTextFromPolyOrder();
    //Reg exp
    void regExpInterManagement(const int &state);
    void regExp();


private:
    void setToolBoxButtonColor(QToolBox* toolBox, int index, QColor color);
    void setCurrentFile(const QString &fileName);
    void setInfoData(const QStringList &fileInfo);
    void setGraph(const QStringList &header, const QVector<QVector<double> > &tab);
    void setGraphParameter();
    void defineAxis(const QStringList &header);
    void setTabCurve(const QString &nameCurve);
    void setTabCurveMathFunction(const QString &nameCurve);
    QVector<QVector<QVector<double> > > convertGraphDataIntoVect(const QVector<QVector<QCPData> > &graphData);
    QColor randomColor(const QString &colorType);
    QString strippedName(const QString &fullFileName);
    //new Cursor
    void createCursorNew();
    void createCursorNewConnection();
    void setCursorVNew(const QString &cursorName, const double &posCursor, const QPen &pen, const double &offset = 0);
    void displayCursor(const QString &name, const QVector<double> &absTab, const QVector<double> &ordTab, const QPen &pen);
    void killCursorNew();
    void killCursorNewConnection();
    void eraseGraphNameFromIndex(const QString &name);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    //new cursor end
    //Curve creation method
    void addItemToComboboxCreateCurve();
    void displayMathFunctionCurve(const QVector<QVector<double> > &tab);
    QString curveName(const QVector<double> &tabId);
    void calculateOffsetTime(const QStringList &fileInfo);
    void addTimeOffsetToGraph();
    void subTimeOffsetToGraph();
    double timeUnit();
    //del base line
    void createCursorInterpol();
    void createCursorInterpolConnection();
    void killCursorInterpol();
    void killCursorInterpolConnection();
    void setCursorV(const QString &cursorName, const double &posCursor, const QPen &pen, const double &offset = 0);
    void setCursorH(const QString &cursorName, const double &posCursor, const QPen &pen, const double &offset = 0);
    void sizeCursorH(const QString &name, QDoubleSpinBox *spinbox);
    void addItemToComboboxInterpol();
    void destroyItemFromCombobox();
    void supressCurveFromIndex(const int &nbCurve);
    void addItemToOpertoionCurve();
    void additemToRegExpComboBox();



    Ui::FenEnfantGraph *ui;
    std::map<QString,int>  indexGraph;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapperTabDelCurve;
    QSignalMapper *signalMapperTabResizeCurve;
    QSignalMapper *signalMapperTabClearCurve;
    QSignalMapper *signalMapperCursorV;
    QSignalMapper *signalMapperCursorH;
    QString curFile;
    QTimer dataTimer;
    bool blackTheme;
    QString graphTitle;
    double offsetTime;
    QStringList m_header;
    bool controlPressed;
    QPen penCursor;
    QPen penCursorInterpolZ1;
    QPen penCursorInterpolZ2;
    MathFunction *mathMethod;
    QStringList m_fileInfo;
    QVector<QVector<double> > m_tabData;
};

#endif // FENENFANTGRAPH_H
