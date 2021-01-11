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
    _iCurrentSamplingRate=DATA_DEFAULT_SAMPLING_RATE;
    _dCurrentGain=DATA_DEFAULT_GAIN;
    _iCurrentDisplayTimespan=DATA_DEFAULT_DISPLAY_TIME_SPAN;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
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
    _iCurrentSamplingRate=iSamplingRateInHz;
    _dCurrentGain=dGainInMultiple;
    _iCurrentDisplayTimespan=iDisplayTimespanInMillisecond;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
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
    _iCurrentSamplingRate=iSamplingRateInHz;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
}

int DataSourceProvider::GetCurrentSamlingRate(){
    return _iCurrentSamplingRate;
}

void DataSourceProvider::SetCurrentGainInMultiple(double dGainInMultiple){
    if (dGainInMultiple<=0){
        cerr<<"DataSourceProvider: Invalid gain, using default value 6.02 dB (x2)."<<endl;
        dGainInMultiple=2;
    }
    _dCurrentGain=dGainInMultiple;
}

double DataSourceProvider::GetCurrentGainInMultiple(){
    return _dCurrentGain;
}

void DataSourceProvider::SetCurrentGainInDb(double dGainInDb){
    _dCurrentGain=DbToMultiple(dGainInDb);
}

double DataSourceProvider::GetCurrentGainInDb(){
    return MultipleToDb(_dCurrentGain);
}

void DataSourceProvider::SetCurrentDisplayTimespan(int iDisplayTimespanInMillisecond){
    if (iDisplayTimespanInMillisecond<=0){
        cerr<<"DataSourceProvider: Invalid display timespan, using default value 1000 ms."<<endl;
        iDisplayTimespanInMillisecond=1000;
    }
    _iCurrentDisplayTimespan=iDisplayTimespanInMillisecond;
    _iPointsPerPlot=(double(_iCurrentDisplayTimespan)/double(1000))*_iCurrentSamplingRate;
}

int DataSourceProvider::GetCurrentDisplayTimespan(){
    return _iCurrentDisplayTimespan;
}

int DataSourceProvider::GetCurrentPointsPerPlot(){
    return _iPointsPerPlot;
}

void DataSourceProvider::SetGate1Parameters(int iGateBeginInMillisecond, int iGateEndInMillisecond){
    _gtpGate1Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    _gtpGate1Info.iGateEndInMillisecond=min(iGateEndInMillisecond,_iCurrentDisplayTimespan);
    _gtpGate1Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(_iCurrentDisplayTimespan),double(0));
    _gtpGate1Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(_iCurrentDisplayTimespan),double(1));
}

const GateParameters & DataSourceProvider::GetGate1Parameters(){
    return _gtpGate1Info;
}

void DataSourceProvider::SetGate2Parameters(int iGateBeginInMillisecond, int iGateEndInMillisecond){
    _gtpGate2Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    _gtpGate2Info.iGateEndInMillisecond=min(iGateEndInMillisecond,_iCurrentDisplayTimespan);
    _gtpGate2Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(_iCurrentDisplayTimespan),double(0));
    _gtpGate2Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(_iCurrentDisplayTimespan),double(1));
}

const GateParameters & DataSourceProvider::GetGate2Parameters(){
    return _gtpGate2Info;
}

QString DataSourceProvider::SamplingRateToString(bool IsUnitTranslationEnabled){
    if (IsUnitTranslationEnabled){
        if (_iCurrentSamplingRate>=UNIT_G){
            return QString::number(double(_iCurrentSamplingRate)/double(UNIT_G),'g',5)+QString(" GHz");
        }
        else if (_iCurrentSamplingRate>=UNIT_M){
            return QString::number(double(_iCurrentSamplingRate)/double(UNIT_M),'g',5)+QString(" MHz");
        }
        else if (_iCurrentSamplingRate>=UNIT_K){
            return QString::number(double(_iCurrentSamplingRate)/double(UNIT_K),'g',5)+QString(" kHz");
        }
        else{
            return QString::number(_iCurrentSamplingRate)+QString(" Hz");
        }
    }
    else{
        return QString::number(_iCurrentSamplingRate)+QString(" Hz");
    }
}

QString DataSourceProvider::SamplingRateToString(int iSamplingRateInHz, bool IsUnitTranslationEnabled){
    iSamplingRateInHz=abs(iSamplingRateInHz);
    if (IsUnitTranslationEnabled){
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

QString DataSourceProvider::GainToString(bool IsDbEnabled, bool IsDbOnly){
    if (IsDbOnly){
        return QString::number(DataSourceProvider::MultipleToDb(_dCurrentGain),'g',4)+QString(" dB");
    }
    if (IsDbEnabled){
        return QString("x")+QString::number(_dCurrentGain,'g',2)+QString(" (")+QString::number(DataSourceProvider::MultipleToDb(_dCurrentGain),'g',4)+QString(" dB)");
    }
    else{
        return QString("x")+QString::number(_dCurrentGain,'g',2);
    }
}

QString DataSourceProvider::GainToString(double dGainInMultiple, bool IsDbEnabled, bool IsDbOnly){
    if (dGainInMultiple<=0){
        return QString("x0");
    }
    if (IsDbOnly){
        return QString::number(DataSourceProvider::MultipleToDb(dGainInMultiple),'g',4)+QString(" dB");
    }
    if (IsDbEnabled){
        return QString("x")+QString::number(dGainInMultiple,'g',2)+QString(" (")+QString::number(DataSourceProvider::MultipleToDb(dGainInMultiple),'g',4)+QString(" dB)");
    }
    else{
        return QString("x")+QString::number(dGainInMultiple,'g',2);
    }
}

QString DataSourceProvider::DisplayTimespanToString(){
    return QString::number(_iCurrentDisplayTimespan)+QString(" ms");
}

QString DataSourceProvider::DisplayTimespanToString(int iDisplayTimespanInMillisecond){
    return QString::number(abs(iDisplayTimespanInMillisecond))+QString(" ms");
}

const QVector<double> & DataSourceProvider::GenerateDAC1(){
    _arrDAC1.clear();
    for (int i=1; i<=_iPointsPerPlot; ++i){
        _arrDAC1.push_back(((double(rand())/double(__INT_MAX__)+5)*_dCurrentGain));
    }
    return _arrDAC1;
}

const QVector<double> & DataSourceProvider::GenerateDAC2(){
    _arrDAC2.clear();
    for (int i=0; i<_iPointsPerPlot; ++i){
        _arrDAC2.push_back(_arrDAC1[i]*2);
    }
    return _arrDAC2;
}

const QVector<double> & DataSourceProvider::GenerateDAC3(){
    _arrDAC3.clear();
    for (int i=0; i<_iPointsPerPlot; ++i){
        _arrDAC3.push_back(_arrDAC1[i]/2);
    }
    return _arrDAC3;
}

const QVector<double> & DataSourceProvider::GenerateGate1(int iGateYValue, int iGateBeginInMillisecond, int iGateEndInMillisecond){
    _arrGate1.clear();
    _gtpGate1Info.iGateYValue=iGateYValue;
    _gtpGate1Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    _gtpGate1Info.iGateEndInMillisecond=min(iGateEndInMillisecond,_iCurrentDisplayTimespan);
    _gtpGate1Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(_iCurrentDisplayTimespan),double(0));
    _gtpGate1Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(_iCurrentDisplayTimespan),double(1));
    for (int i=1; i<=_iPointsPerPlot; ++i){
        _arrGate1.push_back(iGateYValue);
    }
    return _arrGate1;
}

const QVector<double> & DataSourceProvider::GenerateGate2(int iGateYValue, int iGateBeginInMillisecond, int iGateEndInMillisecond){
    _arrGate2.clear();
    _gtpGate2Info.iGateYValue=iGateYValue;
    _gtpGate2Info.iGateBeginInMillisecond=max(iGateBeginInMillisecond,0);
    _gtpGate2Info.iGateEndInMillisecond=min(iGateEndInMillisecond,_iCurrentDisplayTimespan);
    _gtpGate2Info.dGateBeginInPercentage=max(double(iGateBeginInMillisecond)/double(_iCurrentDisplayTimespan),double(0));
    _gtpGate2Info.dGateEndInPercentage=min(double(iGateEndInMillisecond)/double(_iCurrentDisplayTimespan),double(1));
    for (int i=1; i<=_iPointsPerPlot; ++i){
        _arrGate2.push_back(iGateYValue);
    }
    return _arrGate2;
}

const QVector<double> & DataSourceProvider::GeneratePlotForTesting(){
    _arrData.clear();
    for (int i=1; i<=_iPointsPerPlot; ++i){
        _arrData.push_back((double(rand())/double(__INT_MAX__)+2.5*sin(5.0*3.1415926*i/_iPointsPerPlot)+5)*_dCurrentGain);
    }
    return _arrData;
}
