#include "MathFunction.h"
#include <fftw3.h>
#include <QDebug>
#include <QtMath>
#include "overhauser.hpp"
#include "reglin.h"
#include "qcustomplot.h"

//Id of math function
#define AVERAGE_FILTER          0
#define MEDIAN_FILTER           1
#define FFT_FILTER              3
#define REG                     4
#define SPLINE                  5
#define DEL_BASE_LINE           6
#define MOUVING_AVERAGE_FILTER  7
#define MOUVING_MEDIAN_FILTER   8
#define OPERATION_CURVE         9
#define REG_EXP                 10

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
QVector<QVector<double> > MathFunction::averageValueCurve(const QVector<QCPData> &tabData, const int &sampleTime , int startValue, int endValue)
{
    QVector<QVector<double> >dataReturn;

    QVector<QVector<double> > data(convertQCPDataInQVector(tabData));

    if(startValue == -1 && endValue ==-1)
    {
        dataReturn = proceedAverageFilterNew(data, sampleTime, 0, data.at(0).size()-1);
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTime, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTime, 1));
        //long int startValueInd(returnIndOfValueKey(startValue, data.at(0), sampleTime, true));
        //long int endValueInd(returnIndOfValueKey(endValue, data.at(0), sampleTime, false));

        dataReturn = proceedAverageFilterNew(data, sampleTime, startValueInd, endValueInd);
    }

    return createTabReturn(AVERAGE_FILTER, dataReturn.at(0), dataReturn.at(1));
}

QVector<QVector<double> > MathFunction::proceedAverageFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTime, const int &startValueInd, const int &endValueInd)
{
    QVector<QVector<double> > tabDataReturn;

    QVector<double> absTab;
    QVector<double> ordTab;

    double sumAbs(0);
    double sumOrd(0);

    for(long int i(1 + startValueInd); i<= endValueInd+1 ; i++)
    {
        sumAbs += tabData.at(0).at(i-1);
        sumOrd += tabData.at(1).at(i-1);

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

/*** Mouving Average Filter ****/
QVector<QVector<double> > MathFunction::mouvingAverageValueCurveNew(const QVector<QCPData> &tabData, const int &sampleTime , int startValue, int endValue)
{
    int sampleTimeModif(sampleTime);
    if(sampleTime % 2 == 0)
    {
        sampleTimeModif++;
    }
    QVector<QVector<double> > data(convertQCPDataInQVector(tabData));
    QVector<QVector<double> >dataReturn;

    if(startValue == -1 && endValue ==-1)
    {
        dataReturn = proceedMouvingAverageFilterNew(data, sampleTimeModif, 0, data.at(0).size()-1);
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTimeModif, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTimeModif, 1));
        //long int startValueInd(returnIndOfValueKey(startValue, data.at(0), sampleTimeModif, true));
        //long int endValueInd(returnIndOfValueKey(endValue, data.at(0), sampleTimeModif, false));

        dataReturn = proceedMouvingAverageFilterNew(data, sampleTimeModif, startValueInd, endValueInd);
    }

    return createTabReturn(MOUVING_AVERAGE_FILTER, dataReturn.at(0), dataReturn.at(1));
}

QVector<QVector<double> > MathFunction::proceedMouvingAverageFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTime, const int &startValueInd, const int &endValueInd)
{
    QVector<QVector<double> > tabDataReturn;

    QVector<double> ordTab;

    double sumOrd(0);
    for(long int i(1 + startValueInd); i <= (endValueInd+1 - sampleTime); i++)
    {
        for (int j(0 + i); j< (sampleTime + i); j++)
        {
            sumOrd += tabData.at(1).at(j-1);
        }

        ordTab.push_back(sumOrd/sampleTime);

        sumOrd = 0;
    }

    QVector<double> absTab;
    for (long int i(startValueInd + (int)((double)sampleTime/2)); i <= (endValueInd +1 - sampleTime); i++)
    {
        absTab.push_back(tabData.at(0).at(i));
    }

    tabDataReturn.push_back(absTab);
    tabDataReturn.push_back(ordTab);

    return tabDataReturn;
}
/*** Average Value Filter end ****/

/*** Middle Value Filter ****/
QVector<QVector<double> > MathFunction::middleValueCurveFilter(const QVector<QCPData> &tabData, const int &sampleTime, int startValue, int endValue)
{
    int sampleTimeModif(sampleTime);
    if(sampleTime % 2 == 0)
    {
        sampleTimeModif++;
    }

    QVector<QVector<double> > dataReturn;
    QVector<QVector<double> > data(convertQCPDataInQVector(tabData));

    if(startValue == -1 && endValue ==-1)
    {
        dataReturn = proceedMidFilterNew(data, sampleTimeModif, 0, data.at(0).size()-1);
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTimeModif, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTimeModif, 1));

        dataReturn = proceedMidFilterNew(data, sampleTimeModif, startValueInd, endValueInd);
    }

    return createTabReturn(MEDIAN_FILTER, dataReturn.at(0), dataReturn.at(1));
}

QVector<QVector<double> > MathFunction::proceedMidFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTimeModif, const int &startValueInd, const int &endValueInd)
{
    QVector<QVector<double> > tabDataReturn;
    QVector<double> absTab;
    QVector<double> ordTab;

    QVector<double> midAbs(0);
    QVector<double> midOrd(0);

    for(long int i(1 + startValueInd); i<= endValueInd+1 ; i++)
    {
        midAbs.push_back(tabData.at(0).at(i-1));
        midOrd.push_back(tabData.at(1).at(i-1));

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

/*** Mouving Median Filter ****/
QVector<QVector<double> > MathFunction::mouvingMedianValueCurveNew(const QVector<QCPData> &tabData, const int &sampleTime , int startValue, int endValue)
{
    int sampleTimeModif(sampleTime);
    if(sampleTime % 2 == 0)
    {
        sampleTimeModif++;
    }

    QVector<QVector<double> >dataReturn;
    QVector<QVector<double> > data(convertQCPDataInQVector(tabData));

    if(startValue == -1 && endValue ==-1)
    {
        dataReturn = proceedMouvingMedianFilterNew(data, sampleTimeModif, 0, data.at(0).size()-1);
    }
    else
    {
        long int startValueInd(returnIndOfValueAbs(startValue, sampleTimeModif, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, sampleTimeModif, 1));
        //long int startValueInd(returnIndOfValueKey(startValue, data.at(0), sampleTimeModif, true));
        //long int endValueInd(returnIndOfValueKey(endValue, data.at(0), sampleTimeModif, false));

        dataReturn = proceedMouvingMedianFilterNew(data, sampleTimeModif, startValueInd, endValueInd);
    }

    return createTabReturn(MOUVING_MEDIAN_FILTER, dataReturn.at(0), dataReturn.at(1));
}

QVector<QVector<double> > MathFunction::proceedMouvingMedianFilterNew(const QVector<QVector<double> > &tabData, const int &sampleTime, const int &startValueInd, const int &endValueInd)
{
    QVector<QVector<double> > tabDataReturn;

    QVector<double> ordTab;

    QVector<double> midOrd(0);
    for(long int i(1 + startValueInd); i <= (endValueInd+1 - sampleTime); i++)
    {
        for (int j(0 + i); j< (sampleTime + i); j++)
        {
            midOrd.push_back(tabData.at(1).at(j-1));
        }

        std::sort(midOrd.begin(), midOrd.end());

        ordTab.push_back(midOrd.at(sampleTime/2));

        midOrd.clear();
    }

    QVector<double> absTab;
    for (long int i(startValueInd + (int)((double)sampleTime/2)); i <= (endValueInd +1 - sampleTime); i++)
    {
        absTab.push_back(tabData.at(0).at(i));
    }

    tabDataReturn.push_back(absTab);
    tabDataReturn.push_back(ordTab);

    return tabDataReturn;
}
/*** Mouving Median Filter end ****/

/*** FFT Filter ****/
QVector<QVector<double> > MathFunction::fftFilterNew(const QVector<QCPData> &tabData, const int &percentFiltering, int startValue, int endValue)
{
    int mode(0);
    /******** Create a Tab Data **********/
    createTabDataNew(tabData, startValue, endValue);

    /******** FFT Forward **********/
    proceedFFT();

    /******** FFT Filtering **********/
    filteringFFT(percentFiltering);

    /******** FFT Backward **********/
    proceedFFT();

    /******** Tab Data Return **********/
    QVector<QVector<double> > data(tabReturnFFT(tabData, mode, startValue, endValue));
    QVector<double>xAxis(data.at(0));
    QVector<double>yAxis(data.at(1));

    /******** Clean Tab **********/
    endFFT();

    return createTabReturn(FFT_FILTER, xAxis, yAxis);
}

void MathFunction::createTabDataNew(const QVector<QCPData> &tabData, int startValue, int endValue)
{
    m_dataFFT.clear();
    m_testFFT.clear();

    QVector<QVector<double> > data(convertQCPDataInQVector(tabData));

    if (startValue != -1 && endValue != -1)
    {
        long int startValueInd(returnIndOfValueAbs(startValue, 1, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, 1, 1));
        //long int startValueInd(returnIndOfValueKey(startValue, data.at(0), 1, true));
        //long int endValueInd(returnIndOfValueKey(endValue, data.at(0), 1, false));

        long int N(endValueInd - startValueInd);
        for (long int i(0); i < N; i++)
        {
            m_dataFFT.push_back(data.at(1).at(i + startValueInd));
        }
    }
    else
    {
        long int N(data.at(0).size());
        for (long int i(0); i < N; i++)
        {
            m_dataFFT.push_back(data.at(1).at(i));
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

QVector<QVector<double> > MathFunction::tabReturnFFT(const QVector<QCPData> &tabData, int mode, const int &startValue, const int &endValue)
{
    QVector<QVector<double> > data(convertQCPDataInQVector(tabData));
    long int N(data.at(0).size());

    QVector<QVector<double> > dataReturn;

    QVector<double> xAxis;

    if (startValue != -1 && endValue != -1)
    {
        long int startValueInd(returnIndOfValueAbs(startValue, 1, 0));
        long int endValueInd(returnIndOfValueAbs(endValue, 1, 1));

        for (long int i(startValueInd); i <= endValueInd; i++)
        {
            xAxis.push_back(data.at(0).at(i));
        }
    }
    else
    {
        for (int i(0); i< N; i++)
        {
            xAxis.push_back(data.at(0).at(i));
        }
    }

    QVector<double> yAxis;
    if (startValue != -1 && endValue != -1)
    {
        for (int i(0); i<m_dataFFT.size(); i++)
        {
            switch(mode)
            {
                case 0:
                {
                    long int N2(m_dataFFT.size());
                    yAxis.push_back(m_dataFFT.at(i) / (2*(N2-1)));
                    break;
                }
                case 1:
                {
                    yAxis.push_back(qSqrt(m_testFFT.at(i)*m_testFFT.at(i)));
                    break;
                }
            }
        }
    }
    else
    {
        for (int i(0); i< N; i++)
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

/*** Reg & Interpol Function ***/
QVector<QVector<double> > MathFunction::generatePoint(const QVector<QCPData> &graphData,const QVector<double> &tabInd)
{
    QVector<QVector<double> > data(convertQCPDataInQVector(graphData));
    //extract data from tabData
    long int startZone1(returnIndOfValueAbs(tabInd.at(0),1,0));
    long int endZone1(returnIndOfValueAbs(tabInd.at(1),1,0));
    long int startZone2(returnIndOfValueAbs(tabInd.at(4),1,0));
    long int endZone2(returnIndOfValueAbs(tabInd.at(5),1,0));

    QVector<double> dataZone1;
    QVector<double> keyZone1;
    for(long int i(startZone1); i < endZone1; i++)
    {
        dataZone1.push_back(data.at(1).at(i));
        keyZone1.push_back(data.at(0).at(i));
    }

    QVector<double> dataZone2;
    QVector<double> keyZone2;
    for(long int i(startZone2); i < endZone2; i++)
    {
        dataZone2.push_back(data.at(1).at(i));
        keyZone2.push_back(data.at(0).at(i));
    }

    //Reg line for data zone 1
    QVector<QVector<double> > tabZone1;
    tabZone1.push_back(keyZone1);
    tabZone1.push_back(dataZone1);
    QVector<QVector<double> > regLineTabZ1(regLine(tabZone1));

    //Reg Line for data zone 2
    QVector<QVector<double> > tabZone2;
    tabZone2.push_back(keyZone2);
    tabZone2.push_back(dataZone2);
    QVector<QVector<double> > regLineTabZ2(regLine(tabZone2));

    //concatenate data to create a tab of point
    QVector<QVector<double> > tabPoint;
    QVector<double> tabPointData;
    QVector<double> tabPointKey;
    for (long int i(0); i< dataZone1.size(); i++)
    {
        tabPointData.push_back(regLineTabZ1.at(1).at(i));
        tabPointKey.push_back(regLineTabZ1.at(0).at(i));
    }
    for (long int i(0); i< dataZone2.size(); i++)
    {
        tabPointData.push_back(regLineTabZ2.at(1).at(i));
        tabPointKey.push_back(regLineTabZ2.at(0).at(i));
    }

    QVector<QVector<double> > tabDataZone;
    tabDataZone.push_back(tabPointKey);
    tabDataZone.push_back(tabPointData);

    tabPoint = regPoly(tabDataZone, m_polyOrder);

    return createTabReturn(REG, tabPoint.at(0), tabPoint.at(1));
}

QVector<QVector<double> > MathFunction::regLine(QVector<QVector<double> > tabData)
{
    double a;
    a = calculatePente(tabData);

    double b;
    b= calculateOrdonnee(tabData);

    //double corr;
    //corr = calculateCorr(tabData);

    QVector<double> tabYAxis;
    for(long int i(0); i < tabData.at(0).size(); i++)
    {
        tabYAxis.push_back( a*tabData.at(0).at(i) + b);
    }

    QVector<QVector<double> > tabReturn;
    tabReturn.push_back(tabData.at(0));
    tabReturn.push_back(tabYAxis);

    return tabReturn;
}

double MathFunction::calculatePente(QVector<QVector<double> > tabData)
{
    long int N(tabData.at(0).size());
    double X[N];
    for (long int i(0); i< N; i++)
    {
        X[i] = tabData.at(0).at(i);
    }

    double Y[N];
    for (long int i(0); i< N; i++)
    {
        Y[i] = tabData.at(1).at(i);
    }

    double a;
    a = pente(X, Y, N);

    return a;
}

double MathFunction::calculateOrdonnee(QVector<QVector<double> > tabData)
{
    long int N(tabData.at(0).size());
    double X[N];
    for (long int i(0); i< N; i++)
    {
        X[i] = tabData.at(0).at(i);
    }

    double Y[N];
    for (long int i(0); i< N; i++)
    {
        Y[i] = tabData.at(1).at(i);
    }

    double b;
    b = ordonnee(X, Y, N);

    return b;
}

double MathFunction::calculateCorr(QVector<QVector<double> > tabData)
{
    long int N(tabData.size());
    double X[N];
    for (long int i(0); i< N; i++)
    {
        X[i] = tabData.at(0).at(i);
    }

    double Y[N];
    for (long int i(0); i< N; i++)
    {
        Y[i] = tabData.at(1).at(i);
    }

    double cor;
    cor = corr(X, Y, N);

    return cor;
}

QVector<QVector<double> > MathFunction::generateSpline(QVector<QVector<double> > pointTab)
{
    CRSpline *spline = new CRSpline();
    QVector<QVector<double> > tabSpline;
    QVector<QVector<double> > tabReturn;

    addPoint(pointTab, spline);

    tabSpline = generateSplineF(spline);

    tabReturn = fftFilteringSpline(tabSpline, 0);

    return createTabReturn(SPLINE, tabReturn.at(0), tabReturn.at(1));
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

QVector<QVector<double> > MathFunction::generateSplineF(CRSpline *spline)
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

QVector<QVector<double> > MathFunction::regPoly(const QVector<QVector<double> > &tabData, const int &nOrder)
{
    m_regFactor.clear();

    //Generate a matrix of regression
    QVector<QVector<double> > M;
    M = matrixReg(tabData, nOrder);

    //Solve the matrix and return factor for the polynomial function
    m_regFactor = gaussPivot(M);

    //Create a tab of value with the factor
    QVector<QVector<double> > tabReg;
    tabReg = createRegTab(tabData.at(0), m_regFactor);

    //Calculate the corelation coefficient
    double rSquare;
    rSquare = coeffReg(tabData, m_regFactor);
    QVector<double> rSquareTab;
    rSquareTab.push_back(rSquare);
    tabReg.push_back(rSquareTab);

    return tabReg;
}

QVector<QVector<double> > MathFunction::matrixReg(const QVector<QVector<double> > &tabData, const int &nOrder)
{
    QVector<double> xData(tabData.at(0));
    QVector<double> yData(tabData.at(1));

    QVector<QVector<double> > M(nOrder+1);

    for (int c(0); c <=nOrder; c++)
    {
        for (int r(0); r <=nOrder; r++)
        {
            double sum(0);
            for(int k(0); k < xData.size(); k++)
            {
                sum += qPow(xData.at(k), r+c);
            }
            M[c].push_back(sum);
        }
    }

    QVector<double> V;
    for (int r(0); r<=nOrder; r++)
    {
        double sum(0);
        for(int k(0); k < xData.size(); k++)
        {
            sum += qPow(xData.at(k), r) * yData.at(k);
        }
        V.push_back(sum);
    }

    for (int i(0); i< M.size(); i++)
    {
        M[i].push_back(V.at(i));
    }

    return M;
}

QVector<double> MathFunction::gaussPivot(QVector<QVector<double> > tabM)
{    
    int n = tabM.size();

    for (int i(0); i<n; i++)
    {
        // Search for maximum in this column
        double maxEl = abs(tabM.at(i).at(i));
        int maxRow(i);
        for (int k(i+1); k<n; k++)
        {
            if (abs(tabM.at(k).at(i)) > maxEl)
            {
                maxEl = abs(tabM.at(k).at(i));
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        for (int k(i); k<n+1; k++)
        {
            double tmp(tabM[maxRow][k]);
            tabM[maxRow][k] = tabM[i][k];
            tabM[i][k] = tmp;
        }


        // Make all rows below this one 0 in current column
        for (int k=i+1; k<n; k++)
        {
            double c(-tabM.at(k).at(i)/tabM.at(i).at(i));
            for (int j=i; j<n+1; j++)
            {
                if (i==j)
                {
                    tabM[k][j] = 0;

                }
                else
                {
                    tabM[k][j] += c * tabM.at(i).at(j);
                }
            }
        }
    }

    // Solve equation Ax=b for an upper triangular matrix A
    QVector<double> x(n);
    for (int i(n-1); i>=0; i--)
    {
        x[i] = tabM[i][n]/tabM[i][i];
        for (int k(i-1); k>=0; k--)
        {
            tabM[k][n] -= tabM[k][i] * x[i];
        }
    }
    return x;
}

QVector<QVector<double> > MathFunction::createRegTab(const QVector<double> &xTab, const QVector<double> factorReg)
{
    long int startInd(returnIndOfValueAbs(xTab.at(0),1,0));
    long int endInd(returnIndOfValueAbs(xTab.at(xTab.size()-1),1,0));

    // create a tab of xData between start of the first cursor zone and the last cursor of zone 2
    QVector<double> xAxis;
    for (int i(startInd); i<=endInd; i++)
    {
        xAxis.push_back(m_tabData.at(0).at(i));
    }

    QVector<double> yAxis;
    for (int i(0); i<xAxis.size(); i++)
    {
        double sum(0);
        for (int j(0); j<factorReg.size(); j++)
        {
            sum += factorReg.at(j) * qPow(xAxis.at(i), j);
        }
        yAxis.push_back(sum);
    }

    QVector<QVector<double> > tabReturn;
    tabReturn.push_back(xAxis);
    tabReturn.push_back(yAxis);

    return tabReturn;
}

double MathFunction::coeffReg(const QVector<QVector<double> > &tabData, const QVector<double> &factorReg)
{
    m_rSquare = 0;
    //Create a Regression Table
    QVector<double> f;
    for (int i(0); i<tabData.at(0).size(); i++)
    {
        double sum(0);
        for (int j(0); j<factorReg.size(); j++)
        {
            sum += factorReg.at(j) * qPow(tabData.at(0).at(i), j);
        }
        f.push_back(sum);
    }

    QVector<double> y(tabData.at(1));

    double yBar(0);
    for(int i(0); i<y.size(); i++)
    {
        yBar += y.at(i);
    }
    yBar = yBar/y.size();

    double SSres;
    for (int i(0); i<y.size(); i++)
    {
        SSres += qPow(y.at(i) - f.at(i), 2);
    }

    double SStot(0.001);
    for (int i(0); i<y.size(); i++)
    {
        SStot += qPow(y.at(i) - yBar, 2);
    }

    double rSquare(0);
    rSquare = 1-(SSres/SStot);

    m_rSquare = rSquare;
    return rSquare;
}

QVector<double> MathFunction::getRegFactor()
{
    return m_regFactor;
}

double MathFunction::getRegCC()
{
    return m_rSquare;
}

void MathFunction::setPolyOrder(const int &order)
{
    m_polyOrder = order;
}
/*** Reg & Interpol Function end***/

/*** Reg exponential Function ***/
QVector<QVector<double> > MathFunction::createRegExp(const QVector<QCPData> &graphData, const QVector<double> &CursorData, const int &nbPoint)
{
    m_regFactor_Reg_Exp.clear();

    QVector<QVector<double> > dataIn;
    dataIn = createTabForRegExp(graphData, CursorData);

    //Generate a matrix of regression
    QVector<QVector<double> > M;
    M = matrixReg(dataIn, 1);

    //Solve the matrix and return factor for the polynomial function
    m_regFactor_Reg_Exp = gaussPivot(M);

    QVector<QVector<double> > dataOut;
    dataOut = returnTabForRegExp(dataIn, nbPoint, graphData);

    return createTabReturn(REG_EXP, dataOut.at(0), dataOut.at(1));
}

QVector<QVector<double> > MathFunction::createTabForRegExp(const QVector<QCPData> &graphData, const QVector<double> &CursorData)
{
    QVector<QVector<double> > data(convertQCPDataInQVector(graphData));

    long int startInd(returnIndOfValueAbs(CursorData.at(0),1,0));
    long int endInd(returnIndOfValueAbs(CursorData.at(1),1,0));

    QVector<double> xTab;
    QVector<double> yTab;
    for (int i(startInd); i<=endInd; i++)
    {
        xTab.push_back(data.at(0).at(i));
        yTab.push_back(qLn(data.at(1).at(i)));
    }

    QVector<QVector<double> > regExpTab;
    regExpTab.push_back(xTab);
    regExpTab.push_back(yTab);

    return regExpTab;
}

QVector<QVector<double> > MathFunction::returnTabForRegExp(const QVector<QVector<double> > &data, const int &nbPoint, const QVector<QCPData> &graphData)
{
    double deltaT(data.at(0).at(1) - data.at(0).at(0));

    //Get coeff for the equation y=a*exp(b)
    double a(qExp(m_regFactor_Reg_Exp.at(0)));
    double b(m_regFactor_Reg_Exp.at(1));

    QVector<double> xTab;
    QVector<double> yTab;

    QVector<QVector<double> > dataGraph(convertQCPDataInQVector(graphData));
    long int endInd(returnIndOfValueAbs(data.at(0).at(data.at(0).size() -1),1,0));

    for (int i(0); i<endInd; i++)
    {
        xTab.push_back(dataGraph.at(0).at(i));
        yTab.push_back(dataGraph.at(1).at(i));
    }

    for (int i(0); i<nbPoint; i++)
    {
        double x(data.at(0).at(data.at(0).size()-1) + i*deltaT);
        xTab.push_back(x);
        yTab.push_back(a*qExp(b*x));
    }

    QVector<QVector<double> >dataReturn;
    dataReturn.push_back(xTab);
    dataReturn.push_back(yTab);

    return dataReturn;
}

/*** Reg exponential Function end ***/

/*** Baricenter Function ***/
double MathFunction::baricenterCurve(const QVector<QVector<double> > &tabData)
{
    double num(0);
    for (int i(0); i<tabData.at(0).size(); i++)
    {
        num += tabData.at(0).at(i) * tabData.at(1).at(i);
    }

    double den(0);
    for (int i(0); i<tabData.at(0).size(); i++)
    {
        den += tabData.at(1).at(i);
    }

    double T;
    T = num/den;

    return T;
}
/*** Baricenter Function end***/

/*** Del base line Function ***/
QVector<QVector<double> > MathFunction::delBaseLine(const QVector<QCPData> &graphData, const QVector<QVector<double> > &tabReg)
{    
    long int startReg(returnIndOfValueAbs(tabReg.at(0).at(0),1,0));
    long int endReg(returnIndOfValueAbs(tabReg.at(0).at(tabReg.at(0).size()-1),1,0));

    QVector<double> xAxis;
    QVector<double> yAxis;
    QVector<QVector<double> > data(convertQCPDataInQVector(graphData));

    for (int i(startReg); i< endReg; i++)
    {
        xAxis.push_back(data.at(0).at(i));
        yAxis.push_back(data.at(1).at(i) - tabReg.at(1).at(i - startReg));
    }

    return createTabReturn(DEL_BASE_LINE, xAxis, yAxis);
}
/*** del base line end***/

/***  Math function other ***/
QVector<QVector<double> > MathFunction::zeroNegativePoint(QVector<QVector<double> > tabData)
{
    QVector<QVector<double> > tabreturn;
    for (int i(0); i<tabData.size(); i++)
    {
        for (int j(0); j<tabData.at(i).size(); j++)
        {
            if (tabData[i][j] < 0 )
            {
                tabData[i][j] = 0;
            }
        }
    }
    tabreturn = tabData;
    return tabreturn;
}

QVector<QVector<double> > MathFunction::delBaseLineNorm(QVector<QVector<double> > tabData)
{
    //Calculate cuvre area
    double area(0);
    for(int i(0); i< tabData.at(0).size() -1; i++)
    {
        double absisSub(tabData.at(0).at(i+1)-tabData.at(0).at(i));
        double ordSub(tabData.at(1).at(i) + tabData.at(1).at(i+1));
        area += 0.5 * absisSub * ordSub;
    }

    QVector<QVector<double> > tabreturn;

    for (int j(0); j<tabData.at(1).size(); j++)
    {
            tabData[1][j] /= area;
    }

    tabreturn = tabData;
    return tabreturn;
}

QVector<QVector<double> > MathFunction::decayCompensation(const QVector<double> &key, const QVector<QCPData> &value, const double &timeValue, const int &power, const QString &unit)
{
    //firstly we parse half time information in second
    double halfTime(calculateHalfTime(timeValue, power, unit));

    //convert QCPData into vector tab value
    QVector<QVector<double> > tabReturn;
    QVector<double> tabValue;
    for (int i(0); i<key.size(); i++)
    {
        tabValue.push_back(value.at(i).value);
    }

    //calculate the compensation of radioactive decay
    for (int i(0); i<tabValue.size(); i++)
    {
        double factor((key.at(i)-key.at(0))/halfTime);
        tabValue[i] *= qPow(2, factor);
    }

    tabReturn.push_back(key);
    tabReturn.push_back(tabValue);

    return tabReturn;
}

double MathFunction::calculateHalfTime(const double &timeValue, const int &power, const QString &unit)
{
    double halfTime(0);
    halfTime += timeValue;
    halfTime *= qPow(10, power);

    if (unit == "ms")
    {
        halfTime /= 1000;
    }
    else if (unit == "s")
    {

    }
    else if (unit == "min")
    {
        halfTime *= 60;
    }
    else if (unit == "h")
    {
        halfTime *= 60*60;
    }
    else if (unit == "d")
    {
        halfTime *= 60*60*24;
    }
    else if (unit == "y")
    {
        halfTime *= 60*60*24*365.242199;
    }

    return halfTime;
}

QVector<QVector<double> > MathFunction::opertionCurve(const QVector<double> &key, const QVector<QCPData> &curve1, const QVector<QCPData> &curve2, const QString &operation, const double &startValue, const double &endValue)
{
    //looking for a start value
    int start(0);
    if (curve1.at(0).key<=curve2.at(0).key)
    {
        start = returnIndexOfKey(curve2.at(0).key, key);
    }
    else if (curve1.at(0).key>curve2.at(0).key)
    {
        start = returnIndexOfKey(curve1.at(0).key, key);
    }

    int startInd(returnIndexOfKey((int) startValue, key));

    if (startValue != -1 && endValue != -1)
    {
        if (startInd > start)
        {
            start = startInd;
        }
    }

    //looking for an end value
    int end(0);
    if (curve1.at(curve1.size()-1).key<=curve2.at(curve2.size()-1).key)
    {
        end = returnIndexOfKey(curve2.at(curve2.size()-1).key, key);
    }
    else if (curve1.at(curve1.size()-1).key>curve2.at(curve2.size()-1).key)
    {
        end = returnIndexOfKey(curve1.at(curve1.size()-1).key, key);
    }

    int endInd(returnIndexOfKey((int)endValue, key));
    if (startValue != -1 && endValue != -1)
    {
        if (endInd < end)
        {
            end = endInd;
        }
    }

    QVector<double> keytab;
    //create a tab key
    for (int i(start); i<end; i++)
    {
        keytab.push_back(curve1.at(i).key);
    }

    //create curve 1 && curve 2
    QVector<double> valueC1;
    QVector<double> valueC2;
    for (int i(start); i<end; i++)
    {
        valueC1.push_back(curve1.at(i).value);
        valueC2.push_back(curve2.at(i).value);
    }

    //Proceed operation
    QVector<double> newtab;
    if (operation == "+")
    {
        for (int i(0); i< valueC1.size(); i++)
        {
            newtab.push_back( valueC1.at(i) + valueC2.at(i));
        }
    }
    else if (operation == "-")
    {
        for (int i(0); i< valueC1.size(); i++)
        {
            newtab.push_back( valueC1.at(i) - valueC2.at(i));
        }
    }
    else if (operation == "x")
    {
        for (int i(0); i< valueC1.size(); i++)
        {
            newtab.push_back( valueC1.at(i) * valueC2.at(i));
        }
    }
    else if (operation == "/")
    {
        for (int i(0); i< valueC1.size(); i++)
        {
            newtab.push_back( valueC1.at(i) / valueC2.at(i));
        }
    }

    return createTabReturn(OPERATION_CURVE, keytab, newtab);
}

QVector<double> MathFunction::getMinMaxKeyCurve(const QVector<QCPData> &graphData)
{
    QVector<double> dataReturn;
    dataReturn.push_back(graphData.at(0).key);
    dataReturn.push_back(graphData.at(graphData.size()-1).key);
    return dataReturn;
}

/***  Math function other end ***/

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

long int MathFunction::returnIndOfValueKey(const int &value, const QVector<double> &keyTab, const int &sampleTime, const bool &start)
{
    /****************************************************************
     * This function permit to found witch indice correspond a value
     * *************************************************************/
    long int indice(0);
    for(long int i(0); i<= keyTab.size()-1; i++)
    {
        if(keyTab.at(i) == (double) value)
        {
            indice = i;
            break;
        }
    }

    if (start == true)
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

int MathFunction::returnIndexOfKey(const double &value, const QVector<double> &tabKey)
{
    int ind(0);
    for (int i(0); i<tabKey.size(); i++)
    {
        if (value == tabKey.at(i))
        {
            break;
        }
        else
        {
            ind++;
        }
    }
    return ind;
}

QVector<QVector<double> > MathFunction::convertQCPDataInQVector( const QVector<QCPData> &dataTab)
{
    QVector<QVector<double> > dataVect;
    QVector<double> xData;
    QVector<double> yData;

    for (int i(0); i<dataTab.size(); i++)
    {
        xData.push_back(dataTab.at(i).key);
        yData.push_back(dataTab.at(i).value);
    }

    dataVect.push_back(xData);
    dataVect.push_back(yData);
    return dataVect;
}

/*** General function end ****/
