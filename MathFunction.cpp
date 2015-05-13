#include "MathFunction.h"
#include <fftw3.h>
#include <QDebug>
#include <QtMath>

MathFunction::MathFunction(const QVector<QVector<double> > &tabData): m_tabData(tabData)
{

}

MathFunction::~MathFunction()
{

}

QVector<double> MathFunction::dataInfo()
{
    //Return information about data, data min, data max, size of array
    double dataMinX(m_tabData.at(0).at(0));
    double dataMaxX(m_tabData.at(0).at(m_tabData.at(0).size()-1));
    double dataRange(m_tabData.at(0).at(0) - m_tabData.at(0).at(1));

    QVector<double> dataInfoReturn;
    dataInfoReturn.push_back(dataMinX);
    dataInfoReturn.push_back(dataMaxX);
    dataInfoReturn.push_back(dataRange);

    return dataInfoReturn;
}

/*** Average Value Filter ****/
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
/*** Average Value Filter end ****/

/*** Middle Value Filter ****/
QVector<QVector<double> > MathFunction::middleValueCurveFilterNew(const int &nbTab, const int &sampleTime, int startValue, int endValue)
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
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTimeModif, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTimeModif, 1));

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
/*** Middle Value Filter end ****/

/*** FFT Filter ****/
QVector<QVector<double> > MathFunction::fftFilter(const int &nbTab, const int &percentFiltering, int startValue, int endValue)
{
    int mode(0);
    /******** Create a Tab Data **********/
    createTabData(nbTab, startValue, endValue);

    /******** FFT Forward **********/
    proceedFFT();

    /******** FFT Filtering **********/
    filteringFFT(percentFiltering);

    /******** FFT Backward **********/
    proceedFFT();

    /******** Tab Data Return **********/
    QVector<QVector<double> > data(tabReturnFFT(mode, startValue, endValue));
    QVector<double>xAxis(data.at(0));
    QVector<double>yAxis(data.at(1));

    /******** Clean Tab **********/
    endFFT();

    return createTabReturn(3, xAxis, yAxis);
}

void MathFunction::createTabData(const int &nbTab, int startValue, int endValue)
{
    m_dataFFT.clear();
    m_testFFT.clear();

    if (startValue != -1 && endValue != -1)
    {
        long int startValueInd(returnIndOfValueAbs(startValue, 1, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, 1, 1));

        long int N(endValueInd - startValueInd);
        for (long int i(0); i <= N-1; i++)
        {
            m_dataFFT.push_back(m_tabData.at(nbTab).at(i + startValueInd));
        }
    }
    else
    {
        long int N(m_tabData.at(0).size());
        for (long int i(0); i <= N-1; i++)
        {
            m_dataFFT.push_back(m_tabData.at(nbTab).at(i));
        }
    }
}

void MathFunction::proceedFFT()
{
    long int N(m_dataFFT.size());

    double in[N];
    double out[N];

    for (long int i(0); i <= N-1; i++)
    {
        in[i] = m_dataFFT.at(i);
    }

    fftw_plan my_plan;
    my_plan = fftw_plan_r2r_1d(N, in, out, FFTW_REDFT00, FFTW_ESTIMATE);

    fftw_execute(my_plan);

    m_dataFFT.clear();

    for (long int i(0); i <= N-1; i++)
    {
        m_dataFFT.push_back(out[i]);
    }

    for (long int i(0); i <= N-1; i++)
    {
        m_testFFT.push_back(out[i]);
    }

    fftw_destroy_plan(my_plan);
}

void MathFunction::filteringFFT(const int &percentFiltering)
{
    long int N(m_dataFFT.size());

    double startRow(N*(1-((double) percentFiltering/100)));

    for (long int i((int) startRow); i <= N-1; i++)
    {
        m_dataFFT.replace(i, 0);
    }
}

QVector<QVector<double> > MathFunction::tabReturnFFT(int mode, const int &startValue, const int &endValue)
{
    long int N(m_dataFFT.size());

    QVector<QVector<double> > dataReturn;

    QVector<double> xAxis;

    if (startValue != -1 && endValue != -1)
    {
        long int startValueInd(returnIndOfValueAbs(startValue, 1, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, 1, 1));

        long int N(endValueInd - startValueInd);
        for (long int i(0); i <= N-1; i++)
        {
            xAxis.push_back(m_tabData.at(0).at(i + startValueInd));
        }
    }
    else
    {
        for (int i(0); i<= N-1; i++)
        {
            xAxis.push_back(m_tabData.at(0).at(i));
        }
    }

    QVector<double> yAxis;
    for (int i(0); i<= N-1; i++)
    {
        switch(mode)
        {
            case 0:
            {
                yAxis.push_back(m_dataFFT.at(i) / (2*(N-1)));
                break;
            }
            case 1:
            {
                yAxis.push_back(qSqrt(m_testFFT.at(i)*m_testFFT.at(i)));
                break;
            }
        }
    }

    dataReturn.push_back(xAxis);
    dataReturn.push_back(yAxis);

    return dataReturn;
}

void MathFunction::endFFT()
{
    m_dataFFT.clear();
    m_testFFT.clear();
}
/*** FFT Filter end ****/

/*** General function ****/
long int MathFunction::returnIndOfValueAbs(const int &value, const int &sampleTime, const int &type)
{
    /****************************************************************
     * This function permit to found witch indice correspond a value
     * *************************************************************/

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
/*** General function end ****/
