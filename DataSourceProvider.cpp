#include "DataSourceProvider.h"
#include <iostream>
#include <QVector>
#include <QString>
#include <math.h>
#include <cstdlib>

using namespace std;

DataSourceProvider::DataSourceProvider(){
    cerr<<"DataSourceProvider: Initializing with default values: SamplingRate="<<qPrintable(SamplingRateToString(DATA_DEFAULT_SAMPLING_RATE,true))
        <<", Timespan="<<qPrintable(DisplayTimespanToString(DATA_DEFAULT_DISPLAY_TIME_SPAN))
        <<", Gain="<<qPrintable(GainToString(double(DATA_DEFAULT_GAIN),true,false))<<"."
        <<endl;
    iCurrentSamplingRate=DATA_DEFAULT_SAMPLING_RATE;
    dCurrentGain=DATA_DEFAULT_GAIN;
    iCurrentDisplayTimespan=DATA_DEFAULT_DISPLAY_TIME_SPAN;
    iPointsPerPlot=(double(iCurrentDisplayTimespan)/double(1000))*iCurrentSamplingRate;
    //TODO: Drivers
}

DataSourceProvider::DataSourceProvider(int iSamplingRateInHz, double dGainInMultiple, int iDisplayTimespanInMillisecond){
    cerr<<"DataSourceProvider: Initializing with given values."<<endl;
    if (iSamplingRateInHz<=0){
        cerr<<"DataSourceProvider: Invalid sampling rate, using default value "<<qPrintable(SamplingRateToString(DATA_DEFAULT_SAMPLING_RATE,true))<<"."<<endl;
        iSamplingRateInHz=DATA_DEFAULT_SAMPLING_RATE;
    }
    if (dGainInMultiple<=0){
        cerr<<"DataSourceProvider: Invalid gain, using default value "<<qPrintable(GainToString(double(DATA_DEFAULT_GAIN),true,false))<<"."<<endl;
        dGainInMultiple=DATA_DEFAULT_GAIN;
    }
    if (iDisplayTimespanInMillisecond<=0){
        cerr<<"DataSourceProvider: Invalid display timespan, using default value "<<qPrintable(DisplayTimespanToString(DATA_DEFAULT_DISPLAY_TIME_SPAN))<<"."<<endl;
        iDisplayTimespanInMillisecond=DATA_DEFAULT_DISPLAY_TIME_SPAN;
    }
    iCurrentSamplingRate=iSamplingRateInHz;
    dCurrentGain=dGainInMultiple;
    iCurrentDisplayTimespan=iDisplayTimespanInMillisecond;
    iPointsPerPlot=(double(iCurrentDisplayTimespan)/double(1000))*iCurrentSamplingRate;
    //TODO: Drivers
}

DataSourceProvider::~DataSourceProvider(){
    //TODO: Drivers
    cerr<<"DataSourceProvider: Exiting..."<<endl;
}

double DataSourceProvider::MultipleToDb(double dValueInMultiple){
    if (dValueInMultiple<=0){
        return 0;
    }
    return double(20)*log10(dValueInMultiple);
}

double DataSourceProvider::DbToMultiple(double dValueInDb){
    return pow(10,dValueInDb/double(20));
}

void DataSourceProvider::SetCurrentSamplingRate(int iSamplingRateInHz){
    if (iSamplingRateInHz<=0){
        cerr<<"DataSourceProvider: Invalid sampling rate, using default value 100 Hz."<<endl;
        iSamplingRateInHz=100;
    }
    iCurrentSamplingRate=iSamplingRateInHz;
    iPointsPerPlot=(double(iCurrentDisplayTimespan)/double(1000))*iCurrentSamplingRate;
}

int DataSourceProvider::GetCurrentSamlingRate(){
    return iCurrentSamplingRate;
}

void DataSourceProvider::SetCurrentGainInMultiple(double dGainInMultiple){
    if (dGainInMultiple<=0){
        cerr<<"DataSourceProvider: Invalid gain, using default value 6.02 dB (x2)."<<endl;
        dGainInMultiple=2;
    }
    dCurrentGain=dGainInMultiple;
}

double DataSourceProvider::GetCurrentGainInMultiple(){
    return dCurrentGain;
}

void DataSourceProvider::SetCurrentGainInDb(double dGainInDb){
    dCurrentGain=DbToMultiple(dGainInDb);
}

double DataSourceProvider::GetCurrentGainInDb(){
    return MultipleToDb(dCurrentGain);
}

void DataSourceProvider::SetCurrentDisplayTimespan(int iDisplayTimespanInMillisecond){
    if (iDisplayTimespanInMillisecond<=0){
        cerr<<"DataSourceProvider: Invalid display timespan, using default value 1000 ms."<<endl;
        iDisplayTimespanInMillisecond=1000;
    }
    iCurrentDisplayTimespan=iDisplayTimespanInMillisecond;
    iPointsPerPlot=(double(iCurrentDisplayTimespan)/double(1000))*iCurrentSamplingRate;
}

int DataSourceProvider::GetCurrentDisplayTimespan(){
    return iCurrentDisplayTimespan;
}

int DataSourceProvider::GetCurrentPointsPerPlot(){
    return iPointsPerPlot;
}

void DataSourceProvider::SetGate1Parameters(int iGateBeginInMillisecond, int iGateEndInMillisecond){
    gtpGate1Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    gtpGate1Info.iGateEndInMillisecond=min(iGateEndInMillisecond,iCurrentDisplayTimespan);
    gtpGate1Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(iCurrentDisplayTimespan),double(0));
    gtpGate1Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(iCurrentDisplayTimespan),double(1));
}

const GateParameters & DataSourceProvider::GetGate1Parameters(){
    return gtpGate1Info;
}

void DataSourceProvider::SetGate2Parameters(int iGateBeginInMillisecond, int iGateEndInMillisecond){
    gtpGate2Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    gtpGate2Info.iGateEndInMillisecond=min(iGateEndInMillisecond,iCurrentDisplayTimespan);
    gtpGate2Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(iCurrentDisplayTimespan),double(0));
    gtpGate2Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(iCurrentDisplayTimespan),double(1));
}

const GateParameters & DataSourceProvider::GetGate2Parameters(){
    return gtpGate2Info;
}

QString DataSourceProvider::SamplingRateToString(bool bIsUnitTranslationEnabled){
    if (bIsUnitTranslationEnabled){
        if (iCurrentSamplingRate>=UNIT_G){
            return QString::number(double(iCurrentSamplingRate)/double(UNIT_G),'g',5)+QString(" GHz");
        }
        else if (iCurrentSamplingRate>=UNIT_M){
            return QString::number(double(iCurrentSamplingRate)/double(UNIT_M),'g',5)+QString(" MHz");
        }
        else if (iCurrentSamplingRate>=UNIT_K){
            return QString::number(double(iCurrentSamplingRate)/double(UNIT_K),'g',5)+QString(" kHz");
        }
        else{
            return QString::number(iCurrentSamplingRate)+QString(" Hz");
        }
    }
    else{
        return QString::number(iCurrentSamplingRate)+QString(" Hz");
    }
}

QString DataSourceProvider::SamplingRateToString(int iSamplingRateInHz, bool bIsUnitTranslationEnabled){
    iSamplingRateInHz=abs(iSamplingRateInHz);
    if (bIsUnitTranslationEnabled){
        if (iSamplingRateInHz>=UNIT_G){
            return QString::number(double(iSamplingRateInHz)/double(UNIT_G),'g',5)+QString(" GHz");
        }
        else if (iSamplingRateInHz>=UNIT_M){
            return QString::number(double(iSamplingRateInHz)/double(UNIT_M),'g',5)+QString(" MHz");
        }
        else if (iSamplingRateInHz>=UNIT_K){
            return QString::number(double(iSamplingRateInHz)/double(UNIT_K),'g',5)+QString(" kHz");
        }
        else{
            return QString::number(iSamplingRateInHz)+QString(" Hz");
        }
    }
    else{
        return QString::number(iSamplingRateInHz)+QString(" Hz");
    }
}

QString DataSourceProvider::GainToString(bool bIsDbEnabled, bool bIsDbOnly){
    if (bIsDbOnly){
        return QString::number(DataSourceProvider::MultipleToDb(dCurrentGain),'g',4)+QString(" dB");
    }
    if (bIsDbEnabled){
        return QString("x")+QString::number(dCurrentGain,'g',2)+QString(" (")+QString::number(DataSourceProvider::MultipleToDb(dCurrentGain),'g',4)+QString(" dB)");
    }
    else{
        return QString("x")+QString::number(dCurrentGain,'g',2);
    }
}

QString DataSourceProvider::GainToString(double dGainInMultiple, bool bIsDbEnabled, bool bIsDbOnly){
    if (dGainInMultiple<=0){
        return QString("x0");
    }
    if (bIsDbOnly){
        return QString::number(DataSourceProvider::MultipleToDb(dGainInMultiple),'g',4)+QString(" dB");
    }
    if (bIsDbEnabled){
        return QString("x")+QString::number(dGainInMultiple,'g',2)+QString(" (")+QString::number(DataSourceProvider::MultipleToDb(dGainInMultiple),'g',4)+QString(" dB)");
    }
    else{
        return QString("x")+QString::number(dGainInMultiple,'g',2);
    }
}

QString DataSourceProvider::DisplayTimespanToString(){
    return QString::number(iCurrentDisplayTimespan)+QString(" ms");
}

QString DataSourceProvider::DisplayTimespanToString(int iDisplayTimespanInMillisecond){
    return QString::number(abs(iDisplayTimespanInMillisecond))+QString(" ms");
}

const QVector<DATA_TYPE> & DataSourceProvider::GenerateDAC1(){
    arrDAC1.clear();
    for (int i=1; i<=iPointsPerPlot; ++i){
        arrDAC1.push_back(((double(rand())/double(__INT_MAX__)*10+50)*dCurrentGain));
    }
    return arrDAC1;
}

const QVector<DATA_TYPE> & DataSourceProvider::GenerateDAC2(){
    arrDAC2.clear();
    for (int i=0; i<iPointsPerPlot; ++i){
        arrDAC2.push_back(arrDAC1[i]*2);
    }
    return arrDAC2;
}

const QVector<DATA_TYPE> & DataSourceProvider::GenerateDAC3(){
    arrDAC3.clear();
    for (int i=0; i<iPointsPerPlot; ++i){
        arrDAC3.push_back(arrDAC1[i]/2);
    }
    return arrDAC3;
}

const QVector<DATA_TYPE> & DataSourceProvider::GenerateGate1(int iGateYValue, int iGateBeginInMillisecond, int iGateEndInMillisecond){
    arrGate1.clear();
    gtpGate1Info.iGateYValue=iGateYValue;
    gtpGate1Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    gtpGate1Info.iGateEndInMillisecond=min(iGateEndInMillisecond,iCurrentDisplayTimespan);
    gtpGate1Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(iCurrentDisplayTimespan),double(0));
    gtpGate1Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(iCurrentDisplayTimespan),double(1));
    for (int i=1; i<=iPointsPerPlot; ++i){
        arrGate1.push_back(iGateYValue);
    }
    return arrGate1;
}

const QVector<DATA_TYPE> & DataSourceProvider::GenerateGate2(int iGateYValue, int iGateBeginInMillisecond, int iGateEndInMillisecond){
    arrGate2.clear();
    gtpGate2Info.iGateYValue=iGateYValue;
    gtpGate2Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    gtpGate2Info.iGateEndInMillisecond=min(iGateEndInMillisecond,iCurrentDisplayTimespan);
    gtpGate2Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(iCurrentDisplayTimespan),double(0));
    gtpGate2Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(iCurrentDisplayTimespan),double(1));
    for (int i=1; i<=iPointsPerPlot; ++i){
        arrGate2.push_back(iGateYValue);
    }
    return arrGate2;
}

const QVector<DATA_TYPE> & DataSourceProvider::GeneratePlotForTesting(){
    arrData.clear();
    for (int i=1; i<=iPointsPerPlot; ++i){
        arrData.push_back((double(rand())/double(__INT_MAX__)+40*sin(5.0*3.1415926*i/iPointsPerPlot)+100)*dCurrentGain);
    }
    return arrData;
}
