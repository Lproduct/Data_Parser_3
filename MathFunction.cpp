#include "MathFunction.h"

MathFunction::MathFunction(const QVector<QVector<double> > &tabData): m_tabData(tabData)
{

}

MathFunction::~MathFunction()
{

}
QVector<double> MathFunction::dataInfo()
{
    double dataMinX(m_tabData.at(0).at(0));
    double dataMaxX(m_tabData.at(0).at(m_tabData.at(0).size()-1));
    double dataRange(m_tabData.at(0).at(0) - m_tabData.at(0).at(1));

    QVector<double> dataInfoReturn;
    dataInfoReturn.push_back(dataMinX);
    dataInfoReturn.push_back(dataMaxX);
    dataInfoReturn.push_back(dataRange);

    return dataInfoReturn;
}

QVector<QVector<double> > MathFunction::averageValueCurveNew(const int &nbTab, const int &sampleTime , int startValue, int endValue)
{
    QVector<double> absTab(0);
    QVector<double> ordTab(0);

    double sumAbs(0);
    double sumOrd(0);

    if(startValue == -1 && endValue ==-1)
    {
        for(long int i(1); i<= m_tabData.at(0).size() ; i++)
        {
            sumAbs += m_tabData.at(0)[i-1];
            sumOrd += m_tabData.at(nbTab)[i-1];

            if ( i % sampleTime   == 0)
            {
                absTab.push_back(sumAbs/sampleTime);
                ordTab.push_back(sumOrd/sampleTime);
                sumAbs = 0;
                sumOrd = 0;
            }
        }
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTime, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTime, 1));

        for(long int i(1 + startValueInd); i<= endValueInd+1 ; i++)
        {
            sumAbs += m_tabData.at(0)[i-1];
            sumOrd += m_tabData.at(nbTab)[i-1];

            if ( i % sampleTime   == 0)
            {
                absTab.push_back(sumAbs/sampleTime);
                ordTab.push_back(sumOrd/sampleTime);
                sumAbs = 0;
                sumOrd = 0;
            }
        }
    }

    return createTabReturn(0, absTab, ordTab);
}

QVector<QVector<double> > MathFunction::middleValueCurveFilterNew(const int &nbTab, const int &sampleTime , int startValue, int endValue)
{
    int sampleTimeModif(sampleTime);
    if(sampleTime % 2 == 0)
    {
        sampleTimeModif++;
    }

    QVector<double> absTab;
    QVector<double> ordTab;

    QVector<double> midAbs(0);
    QVector<double> midOrd(0);

    if(startValue == -1 && endValue ==-1)
    {
        for(long int i(1); i<= m_tabData.at(0).size() ; i++)
        {
            midAbs.push_back(m_tabData.at(0)[i-1]);
            midOrd.push_back(m_tabData.at(nbTab)[i-1]);

            if ( i % sampleTimeModif   == 0)
            {
                std::sort(midAbs.begin(), midAbs.end());
                std::sort(midOrd.begin(), midOrd.end());

                absTab.push_back(midAbs.at(sampleTimeModif/2));
                ordTab.push_back(midOrd.at(sampleTimeModif/2));

                midAbs.clear();
                midOrd.clear();
            }
        }
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTime, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTime, 1));

        for(long int i(1 + startValueInd); i<= endValueInd+1 ; i++)
        {
            midAbs.push_back(m_tabData.at(0)[i-1]);
            midOrd.push_back(m_tabData.at(nbTab)[i-1]);

            if ( i % sampleTimeModif   == 0)
            {
                std::sort(midAbs.begin(), midAbs.end());
                std::sort(midOrd.begin(), midOrd.end());

                absTab.push_back(midAbs.at(sampleTimeModif/2));
                ordTab.push_back(midOrd.at(sampleTimeModif/2));

                midAbs.clear();
                midOrd.clear();
            }
        }
    }

    return createTabReturn(1, absTab, ordTab);
}

long int MathFunction::returnIndOfValueAbs(const int &value, const int &sampleTime, const int &type)
{
    long int indice(0);
    for(long int i(0); i<= m_tabData.at(0).size()-1; i++)
    {
        if(m_tabData.at(0).at(i) == (double) value)
        {
            indice = i;
            break;
        }
    }

    if (type == 0)
    {
        if(indice %  sampleTime !=0 )
        {
            double test((int)(indice/sampleTime) + 1);
            indice = sampleTime*test;
        }
    }

    return indice;
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
