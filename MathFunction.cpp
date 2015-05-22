#include "MathFunction.h"
#include <fftw3.h>
#include <QDebug>
#include <QtMath>
#include "overhauser.hpp"

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
    double dataRange( - (m_tabData.at(0).at(0) - m_tabData.at(0).at(1)));
    double nbValue(m_tabData.at(0).size());

    double dataMaxY(m_tabData.at(1).at(0));
    double dataMinY(m_tabData.at(1).at(0));
    for(int j(1); j < m_tabData.size(); j++ )
    {
        for(long int i(0); i < m_tabData.at(0).size(); i++)
        {
            if(m_tabData.at(j).at(i) >= dataMaxY)
            {
                dataMaxY = m_tabData.at(j).at(i);
            }
        }

        for(long int i(0); i < m_tabData.at(0).size(); i++)
        {
            if(m_tabData.at(j).at(i) <= dataMinY)
            {
                dataMinY = m_tabData.at(j).at(i);
            }
        }
    }

    QVector<double> dataInfoReturn;
    dataInfoReturn.push_back(dataMinX);  //0
    dataInfoReturn.push_back(dataMaxX);  //1
    dataInfoReturn.push_back(dataRange); //2
    dataInfoReturn.push_back(nbValue);   //3
    dataInfoReturn.push_back(nbValue);   //4
    dataInfoReturn.push_back(dataMinY);  //5
    dataInfoReturn.push_back(dataMaxY);  //6

    return dataInfoReturn;
}

/*** Average Value Filter ****/
QVector<QVector<double> > MathFunction::averageValueCurveNew(const int &nbTab, const int &sampleTime , int startValue, int endValue)
{
    QVector<double> xAxis;
    QVector<double> yAxis;

    if(startValue == -1 && endValue ==-1)
    {
        QVector<QVector<double> >data(proceedMidFilter(nbTab, sampleTime, 0, m_tabData.at(0).size() -1));
        xAxis = data.at(0);
        yAxis = data.at(1);
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTime, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTime, 1));

        QVector<QVector<double> > data(proceedMidFilter(nbTab, sampleTime, startValueInd, endValueInd));
        xAxis = data.at(0);
        yAxis = data.at(1);
    }

    return createTabReturn(0, xAxis, yAxis);
}

QVector<QVector<double> > MathFunction::proceedAverageFilter(const int &nbTab, const int &sampleTime, const int &startValueInd, const int &endValueInd)
{
    QVector<QVector<double> > tabDataReturn;

    QVector<double> absTab;
    QVector<double> ordTab;

    double sumAbs(0);
    double sumOrd(0);

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

    tabDataReturn.push_back(absTab);
    tabDataReturn.push_back(ordTab);

    return tabDataReturn;
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

    QVector<double> xAxis;
    QVector<double> yAxis;

    if(startValue == -1 && endValue ==-1)
    {
        QVector<QVector<double> >data(proceedMidFilter(nbTab, sampleTimeModif, 0, m_tabData.at(0).size() -1));
        xAxis = data.at(0);
        yAxis = data.at(1);
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTimeModif, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTimeModif, 1));

        QVector<QVector<double> > data(proceedMidFilter(nbTab, sampleTimeModif, startValueInd, endValueInd));
        xAxis = data.at(0);
        yAxis = data.at(1);
    }

    return createTabReturn(1, xAxis, yAxis);
}

QVector<QVector<double> > MathFunction::proceedMidFilter(const int &nbTab, const int &sampleTimeModif, const int &startValueInd, const int &endValueInd)
{
    QVector<QVector<double> > tabDataReturn;
    QVector<double> absTab;
    QVector<double> ordTab;

    QVector<double> midAbs(0);
    QVector<double> midOrd(0);

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

    tabDataReturn.push_back(absTab);
    tabDataReturn.push_back(ordTab);

    return tabDataReturn;
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

    /*if (m_testFFT.size() == 0)
    {
        for (long int i(0); i <= N-1; i++)
        {
            m_testFFT.push_back(out[i]);
        }
    }*/


    fftw_destroy_plan(my_plan);
}

void MathFunction::filteringFFT(const double &percentFiltering)
{
    long int N(m_dataFFT.size());

    double startRow(N*(1-(percentFiltering/100)));

    for (long int i((int) startRow); i <= N-1; i++)
    {
        m_dataFFT.replace(i, 0);
    }

    if (m_testFFT.size() == 0)
    {
        for (long int i(0); i <= N-1; i++)
        {
            m_testFFT.push_back(m_dataFFT.at(i));
        }
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

/*** Spline Function ***/

QVector<QVector<double> > MathFunction::generatePoint(const int &nbCurve,const QVector<double> &tabPoint)
{
    //extract data from tabData
    long int startZone1(returnIndOfValueAbs(tabPoint.at(0),1,0));
    long int endZone1(returnIndOfValueAbs(tabPoint.at(1),1,0));
    long int startZone2(returnIndOfValueAbs(tabPoint.at(4),1,0));
    long int endZone2(returnIndOfValueAbs(tabPoint.at(5),1,0));

    QVector<double> dataZone1;
    QVector<double> keyZone1;
    for(long int i(startZone1); i < endZone1; i++)
    {
        dataZone1.push_back(m_tabData.at(nbCurve).at(i));
        keyZone1.push_back(i);
    }

    QVector<double> dataZone2;
    QVector<double> keyZone2;
    for(long int i(startZone2); i < endZone2; i++)
    {
        dataZone2.push_back(m_tabData.at(nbCurve).at(i));
        keyZone2.push_back(i);
    }



    return createTabReturn(4, xAxis, yAxis);;
}

QVector<QVector<double> > MathFunction::generateSpline(QVector<QVector<double> > pointTab)
{
    CRSpline *spline = new CRSpline();
    QVector<QVector<double> > tabSpline;
    QVector<QVector<double> > tabReturn;

    addPoint(m_tabPoint, spline);

    tabSpline = generateSpline(spline);

    tabReturn = fftFilteringSpline(tabSpline, 80);

    return createTabReturn(5, tabReturn.at(0), tabReturn.at(1));
}

void MathFunction::addPoint(const QVector<QVector<double> > &pointTab, CRSpline *spline)
{
    long int N(pointTab.at(0).size());

    for (int i = 0; i < N; i++)
    {        
        vec3 v(pointTab.at(0).at(i), pointTab.at(1).at(i), 0);
        spline->AddSplinePoint(v);
    }
}

QVector<QVector<double> > MathFunction::generateSpline(CRSpline *spline)
{
    QVector<QVector<double> > tabReturn;
    QVector<double> xAxis;
    QVector<double> yAxis;

    for (int i = 0; i < 2000; i++)
    {
        float t = (float)i / (float)2000;
        vec3 rv = spline->GetInterpolatedSplinePoint(t);
        xAxis.push_back(rv.x);
        yAxis.push_back(rv.y);
    }

    tabReturn.push_back(xAxis);
    tabReturn.push_back(yAxis);

    return tabReturn;
}

QVector<QVector<double> > MathFunction::fftFilteringSpline(QVector<QVector<double> > tabSpline, const double &percentFFT)
{
    int mode(0);

    /******** Create a Tab Data **********/
    createTabDataSpline(tabSpline);

    /******** FFT Forward **********/
    proceedFFT();

    /******** FFT Filtering **********/
    filteringFFT(percentFFT);

    /******** FFT Backward **********/
    proceedFFT();

    /******** Tab Data Return **********/
    QVector<QVector<double> > tabSplineFilt(tabReturSplineFFT(tabSpline, mode));

    /******** Clean Tab **********/
    endFFT();

    return tabSplineFilt;
}

void MathFunction::createTabDataSpline(QVector<QVector<double> > tabSpline)
{
    m_dataFFT.clear();
    m_testFFT.clear();

    long int N(tabSpline.at(0).size());
    for (long int i(0); i <= N-1; i++)
    {
        m_dataFFT.push_back(tabSpline.at(1).at(i));
    }
}

QVector<QVector<double> > MathFunction::tabReturSplineFFT(QVector<QVector<double> > tabSpline, int mode)
{
    long int N(m_dataFFT.size());

    QVector<QVector<double> > dataReturn;

    QVector<double> xAxis;

    for (int i(0); i<= N-1; i++)
    {
        xAxis.push_back(tabSpline.at(0).at(i));
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
/*** Spline Function end***/

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
