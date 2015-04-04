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
    void initCursor();
    void setCursorCurveV(const int cursorId, const double &posCursor, const int &nbGraph, const QCPRange &cursorHeight);
    void keyPressEvent(QKeyEvent *event);
    void createCurveIntialisation();
    void displayMathFunctionCurve(const QVector<QVector<double> > &tab);
    QString curveName(const QVector<double> &tabId);

    MathFunction *mathMethod;
    QColor randomColor(const QString &colorType);
    QString strippedName(const QString &fullFileName);
    Ui::FenEnfantGraph *ui;
    std::map<QString,int>  indexGraph;
    QSignalMapper *signalMapper;
    QString curFile;
    QTimer dataTimer;
};

#endif // FENENFANTGRAPH_H
