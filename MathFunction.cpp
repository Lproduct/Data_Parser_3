#include "MathFunction.h"

MathFunction::MathFunction(const QVector<QVector<double> > &tabData): m_tabData(tabData)
{

}

MathFunction::~MathFunction()
{

}

QVector<QVector<double> > MathFunction::middleValueCurveFilter(const int nbTab, const int &sampleTime)
{
    //Create a value middle curve
    int sampleTimeModif;
    if (sampleTime%2 == 0)
    {
        sampleTimeModif = sampleTime +1;
    }
    else
    {
        sampleTimeModif = sampleTime;
    }

    QVector<double> subTab(m_tabData.at(0));

    //int nbColumn(tab.size());
    int nbRow(subTab.size());

    int reste(nbRow%sampleTimeModif);
    int nbLoop(0);
    if (reste == 0)
    {
        nbLoop =nbRow/sampleTimeModif;
    }
    else
    {
        nbLoop = (nbRow-reste)/sampleTimeModif;
    }

    QVector<double> absTab;
    double sampleTimeModifDouble = (double) sampleTimeModif;
    for(int i(0); i<=nbLoop-1; i++)
    {
        absTab.push_back(sampleTimeModifDouble/2+i*sampleTimeModifDouble);
    }

    QVector<double> ordTab;
    QVector<double> subtab2(m_tabData.at(nbTab));
    for(int i(0); i<=nbLoop-1; i++)
    {
        QVector<double> middleTab(0);

        for(int k(0+i*(sampleTimeModif)); k<=sampleTimeModif-1+i*(sampleTimeModif); k++)
        {
            middleTab.push_back(subtab2.at(k));
        }
        std::sort(middleTab.begin(), middleTab.end());
        ordTab.push_back(middleTab.at(sampleTimeModif/2));
    }

    return createTabReturn(0, absTab, ordTab);
}

QVector<QVector<double> > MathFunction::averageValueCurve(const int nbTab, const int &sampleTime)
{
    //Create a average curve
    QVector<double> subTab(m_tabData.at(0));

    //int nbColumn(tab.size());
    int nbRow(subTab.size());

    int reste(nbRow%sampleTime);
    int nbLoop(0);
    if (reste == 0)
    {
        nbLoop =nbRow/sampleTime;
    }
    else
    {
        nbLoop = (nbRow-reste)/sampleTime;
    }

    QVector<double> absTab;
    double sampleTimeModif = (double) sampleTime;
    for(int i(0); i<=nbLoop-1; i++)
    {
        absTab.push_back(sampleTimeModif/2+i*sampleTimeModif);
    }

    QVector<double> ordTab;
    QVector<double> subtab2(m_tabData.at(nbTab));
    for(int i(0); i<=nbLoop-1; i++)
    {
        double average(0);
        for(int k(0+i*(sampleTime)); k<=sampleTime-1+i*(sampleTime); k++)
        {
            average += subtab2.at(k);
        }
        ordTab.push_back(average/sampleTime);
    }

    return createTabReturn(1, absTab, ordTab);
}

QVector<QVector<double> > MathFunction::createTabReturn(const int &OpId, const QVector<double> &absTab, const QVector<double> &ordTab)
{
    QVector<double> tabOpId(0);
    tabOpId.push_back((double)OpId);

    QVector<QVector<double> > dataReturn(0);
    dataReturn.push_back(absTab);
    dataReturn.push_back(ordTab);
    dataReturn.push_back(tabOpId);

    return dataReturn;
}

