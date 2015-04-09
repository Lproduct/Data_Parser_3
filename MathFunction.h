#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include "QtWidgets"

class MathFunction
{
public:
    MathFunction(const QVector<QVector<double> > &tabData);
    ~MathFunction();

    QVector<QVector<double> > middleValueCurveFilter(const int &nbTab, const int &sampleTime, int startValue = -1, int endValue = -1);
    QVector<QVector<double> > averageValueCurve(const int &nbTab, const int &sampleTime, int startValue = -1, int endValue = -1);

private:
    QVector<QVector<double> > createTabReturn(const int &OpId, const QVector<double> &absTab, const QVector<double> &ordTab);

    QVector<QVector<double> > m_tabData;
};

#endif // MATHFUNCTION_H
