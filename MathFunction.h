#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include "QtWidgets"
#include <QtMath>
#include "overhauser.hpp"
#include "qcustomplot.h"

class MathFunction
{
public:
    MathFunction(const QVector<QVector<double> > &tabData);
    ~MathFunction();

    QVector<double> dataInfo();
    QVector<QVector<double> > averageValueCurve(const QVector<QCPData> &tabData, const int &sampleTime , int startValue = -1, int endValue = -1);
    QVector<QVector<double> > middleValueCurveFilter(const QVector<QCPData> &tabData, const int &sampleTime, int startValue  = -1, int endValue = -1);
    QVector<QVector<double> > fftFilter(const int &nbTab, const int &percentFiltering, int startValue = -1, int endValue = -1);
    QVector<QVector<double> > fftFilterNew(const QVector<QCPData> &tabData, const int &percentFiltering, int startValue = -1, int endValue = -1);
    QVector<QVector<double> > generateSpline(QVector<QVector<double> > pointTab);
    QVector<QVector<double> > generatePoint(const QVector<QCPData> &graphData, const QVector<double> &tabInd);
    QVector<QVector<double> > delBaseLine(const QVector<QCPData> &graphData, const QVector<QVector<double> > &tabReg);
    QVector<QVector<double> > mouvingAverageValueCurveNew(const QVector<QCPData> &tabData, const int &sampleTime, int startValue = -1, int endValue = -1);
    QVector<QVector<double> > mouvingMedianValueCurveNew(const QVector<QCPData> &tabData, const int &sampleTime , int startValue= -1, int endValue =-1);
    QVector<double> getRegFactor();
    void setPolyOrder(const int &order);
    double getRegCC();
    double baricenterCurve(const QVector<QVector<double> > &tabData);
    QVector<QVector<double> > zeroNegativePoint(QVector<QVector<double> > tabData);
    QVector<QVector<double> > delBaseLineNorm(QVector<QVector<double> > tabData);
    QVector<QVector<double> > decayCompensation(const QVector<double> &key, const QVector<QCPData> &value, const double &timeValue, const int &power, const QString &unit);
    QVector<QVector<double> > opertionCurve(const QVector<double> &key, const QVector<QCPData> &curve1, const QVector<QCPData> &curve2, const QString &operation, const double &startValue = -1, const double &endValue = -1);

private:
    /*** Average Value Filter ****/
    QVector<QVector<double> > proceedAverageFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTime, const int &startValueInd, const int &endValueInd);
    /*** Average Value Filter end ****/

    /*** Middle Value Filter ****/
    QVector<QVector<double> > proceedMidFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTimeModif, const int &startValueInd, const int &endValueInd);
    /*** Middle Value Filter end****/

    /*** FFT Function ***/
    void createTabDataNew(const QVector<QCPData> &tabData, int startValue, int endValue);
    void proceedFFT();
    void filteringFFT(const double &percentFiltering);
    QVector<QVector<double> > tabReturnFFT(int mode, const int &startValue, const int &endValue);
    void endFFT();    
    /*** FFT Function end ***/

    /*** Reg & Interpol Function ***/
    void addPoint(const QVector<QVector<double> > &pointTab, CRSpline *spline);
    QVector<QVector<double> > generateSplineF(CRSpline *spline);
    QVector<QVector<double> > fftFilteringSpline(QVector<QVector<double> > tabSpline, const double &percentFFT);
    void createTabDataSpline(QVector<QVector<double> > tabSpline);
    QVector<QVector<double> > tabReturSplineFFT(QVector<QVector<double> > tabSpline, int mode);
    /*** Reg & Interpol Function end***/

    /*** General Function ***/
    QVector<QVector<double> > createTabReturn(const int &OpId, const QVector<double> &absTab, const QVector<double> &ordTab);
    long int returnIndOfValueAbs(const int &value, const int &sampleTime, const int &type);
    int returnIndexOfKey(const double &value, const QVector<double> &tabKey);
    QVector<QVector<double> > convertQCPDataInQVector(const QVector<QCPData> &dataTab);
    /*** General Function end ***/

    /*** Reg Line ***/
    QVector<QVector<double> > regLine(QVector<QVector<double> > tabData);
    double calculatePente(QVector<QVector<double> > tabData);
    double calculateOrdonnee(QVector<QVector<double> > tabData);
    double calculateCorr(QVector<QVector<double> > tabData);
    QVector<QVector<double> > regPoly(const QVector<QVector<double> > &tabData, const int &nOrder);
    QVector<QVector<double> > matrixReg(const QVector<QVector<double> > &tabData, const int &nOrder);
    QVector<double> gaussPivot(QVector<QVector<double> > tabM);
    QVector<QVector<double> > createRegTab(const QVector<double> &xTab, const QVector<double> factorReg);
    double coeffReg(const QVector<QVector<double> > &tabData, const QVector<double> &factorReg);
    /*** Reg Line end ***/

    /*** Mouving Average filter ***/
    QVector<QVector<double> > proceedMouvingAverageFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTime, const int &startValueInd, const int &endValueInd);
    /*** Mouving Average filter end***/

    /*** Mouving Median Filter ****/
    QVector<QVector<double> > proceedMouvingMedianFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTime, const int &startValueInd, const int &endValueInd);
    /*** Mouving Median Filter end ****/

    /*** Decay compensation function ****/
    double calculateHalfTime(const double &timeValue, const int &power, const QString &unit);
    /*** Decay compensation function end ****/

    QVector<QVector<double> > m_tabData;
    QVector<QVector<double> > m_tabPoint;
    QVector<double> m_dataFFT;
    QVector<double> m_testFFT;
    int m_polyOrder;
    QVector<double> m_regFactor;
    double m_rSquare;
};

#endif // MATHFUNCTION_H
