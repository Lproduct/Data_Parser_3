#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include "QtWidgets"
#include <QtMath>
#include "overhauser.hpp"

class MathFunction
{
public:
    MathFunction(const QVector<QVector<double> > &tabData);
    ~MathFunction();

    QVector<double> dataInfo();
    QVector<QVector<double> > averageValueCurveNew(const int &nbTab, const int &sampleTime , int startValue = -1, int endValue = -1);
    QVector<QVector<double> > middleValueCurveFilterNew(const int &nbTab, const int &sampleTime , int startValue = -1, int endValue = -1);
    QVector<QVector<double> > fftFilter(const int &nbTab, const int &percentFiltering, int startValue = -1, int endValue = -1);
    QVector<QVector<double> > generateSpline(QVector<QVector<double> > pointTab);
    QVector<QVector<double> > generatePoint(const int &nbCurve, const QVector<double> &tabInd);
    QVector<QVector<double> > delBaseLine(const int &nbCurve, const QVector<QVector<double> > &tabReg);
    QVector<QVector<double> > mouvingAverageValueCurve(const int &nbTab, const int &sampleTime , int startValue = -1, int endValue = -1);
    QVector<QVector<double> > mouvingMedianValueCurve(const int &nbTab, const int &sampleTime , int startValue = -1, int endValue = -1);

private:
    /*** Average Value Filter ****/
    QVector<QVector<double> > proceedAverageFilter(const int &nbTab, const int &sampleTimeModif, const int &startValueInd, const int &endValueInd);
    /*** Average Value Filter end ****/

    /*** Middle Value Filter ****/
    QVector<QVector<double> > proceedMidFilter(const int &nbTab, const int &sampleTimeModif, const int &startValueInd, const int &endValueInd);
    /*** Middle Value Filter end****/

    /*** FFT Function ***/
    void createTabData(const int &nbTab, int startValue, int endValue);
    void proceedFFT();
    void filteringFFT(const double &percentFiltering);
    QVector<QVector<double> > tabReturnFFT(int mode, const int &startValue, const int &endValue);
    void endFFT();
    /*** FFT Function end ***/

    /*** Spline Function ***/
    void addPoint(const QVector<QVector<double> > &pointTab, CRSpline *spline);
    QVector<QVector<double> > generateSplineF(CRSpline *spline);
    QVector<QVector<double> > fftFilteringSpline(QVector<QVector<double> > tabSpline, const double &percentFFT);
    void createTabDataSpline(QVector<QVector<double> > tabSpline);
    QVector<QVector<double> > tabReturSplineFFT(QVector<QVector<double> > tabSpline, int mode);
    /*** Spline Function end***/

    /*** General Function ***/
    QVector<QVector<double> > createTabReturn(const int &OpId, const QVector<double> &absTab, const QVector<double> &ordTab);
    long int returnIndOfValueAbs(const int &value, const int &sampleTime, const int &type);
    /*** General Function end ***/

    /*** Reg Line ***/
    QVector<QVector<double> > regLine(QVector<QVector<double> > tabData);
    double calculatePente(QVector<QVector<double> > tabData);
    double calculateOrdonnee(QVector<QVector<double> > tabData);
    double calculateCorr(QVector<QVector<double> > tabData);
    QVector<QVector<double> > regPoly(const QVector<QVector<double> > &tabData, const int &nOrder);
    QVector<double> gaussPivot(QVector<QVector<double> > tabM, QVector<double> tabV);
    QVector<QVector<double> > createRegTab(const QVector<double> &xTab, const QVector<double> factorReg);
    /*** Reg Line end ***/

    /*** Mouving Average filter ***/
    QVector<QVector<double> > proceedMouvingAverageFilter(const int &nbTab, const int &sampleTime, const int &startValueInd, const int &endValueInd);
    /*** Mouving Average filter end***/

    /*** Mouving Median Filter ****/
    QVector<QVector<double> > proceedMouvingMedianFilter(const int &nbTab, const int &sampleTime, const int &startValueInd, const int &endValueInd);
    /*** Mouving Median Filter end ****/

    QVector<QVector<double> > m_tabData;
    QVector<QVector<double> > m_tabPoint;
    QVector<double> m_dataFFT;
    QVector<double> m_testFFT;
};

#endif // MATHFUNCTION_H
