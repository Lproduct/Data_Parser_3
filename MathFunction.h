#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include "QtWidgets"
#include <QtMath>

class MathFunction
{
public:
    MathFunction(const QVector<QVector<double> > &tabData);
    ~MathFunction();

    QVector<double> dataInfo();
    QVector<QVector<double> > averageValueCurveNew(const int &nbTab, const int &sampleTime , int startValue = -1, int endValue = -1);
    QVector<QVector<double> >middleValueCurveFilterNew(const int &nbTab, const int &sampleTime , int startValue = -1, int endValue = -1);
    QVector<QVector<double> > fftFilter(const int &nbTab, const int &percentFiltering, int startValue = -1, int endValue = -1);

private:
    void createTabData(const int &nbTab, int startValue, int endValue);
    void proceedFFT();
    void filteringFFT(const int &percentFiltering);
    QVector<QVector<double> > tabReturnFFT(int mode, const int &startValue, const int &endValue);
    void endFFT();

    QVector<QVector<double> > createTabReturn(const int &OpId, const QVector<double> &absTab, const QVector<double> &ordTab);
    long int returnIndOfValueAbs(const int &value, const int &sampleTime, const int &type);

    QVector<QVector<double> > m_tabData;
    QVector<double> m_dataFFT;
    QVector<double> m_testFFT;
};

#endif // MATHFUNCTION_H
