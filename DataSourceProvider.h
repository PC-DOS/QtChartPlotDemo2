#ifndef DATASOURCEPROVIDER_H
#define DATASOURCEPROVIDER_H

#include <QVector>
#include <QString>

#define UNIT_G 1000000000
#define UNIT_M 1000000
#define UNIT_K 1000

#define DATA_DEFAULT_SAMPLING_RATE 50000
#define DATA_DEFAULT_GAIN 2
#define DATA_DEFAULT_DISPLAY_TIME_SPAN 20

#define DATA_TYPE int

struct GateParameters{
    int iGateYValue;
    int iGateBeginInMillisecond;
    int iGateEndInMillisecond;
    double dGateBeginInPercentage;
    double dGateEndInPercentage;
};

class DataSourceProvider{
private:
    int _iCurrentSamplingRate;
    double _dCurrentGain; //In multiples, to generate value in dB, use: dB = 20lg(Multiples)
    int _iCurrentDisplayTimespan;
    int _iPointsPerPlot;
    GateParameters _gtpGate1Info;
    GateParameters _gtpGate2Info;

    QVector<DATA_TYPE> _arrData;
    QVector<DATA_TYPE> _arrDAC1;
    QVector<DATA_TYPE> _arrDAC2;
    QVector<DATA_TYPE> _arrDAC3;
    QVector<DATA_TYPE> _arrGate1;
    QVector<DATA_TYPE> _arrGate2;

public:
    DataSourceProvider();
    DataSourceProvider(int iSamplingRateInHz, double dGainInMultiple, int iDisplayTimespanInMillisecond);
    ~DataSourceProvider();

    double MultipleToDb(double dValueInMultiple);
    double DbToMultiple(double dValueInDb);

    void SetCurrentSamplingRate(int iSamplingRateInHz);
    int GetCurrentSamlingRate();
    void SetCurrentGainInMultiple(double dGainInMultiple);
    double GetCurrentGainInMultiple();
    void SetCurrentGainInDb(double dGainInDb);
    double GetCurrentGainInDb();
    void SetCurrentDisplayTimespan(int iDisplayTimespanInMillisecond);
    int GetCurrentDisplayTimespan();
    int GetCurrentPointsPerPlot();
    void SetGate1Parameters(int iGateBeginInMillisecond, int iGateEndInMillisecond);
    const GateParameters & GetGate1Parameters();
    void SetGate2Parameters(int iGateBeginInMillisecond, int iGateEndInMillisecond);
    const GateParameters & GetGate2Parameters();

    QString SamplingRateToString(bool IsUnitTranslationEnabled=true);
    QString SamplingRateToString(int iSamplingRateInHz, bool IsUnitTranslationEnabled=true);
    QString GainToString(bool IsDbEnabled=true, bool IsDbOnly=false);
    QString GainToString(double dGainInMultiple, bool IsDbEnabled=true, bool IsDbOnly=false);
    QString DisplayTimespanToString();
    QString DisplayTimespanToString(int iDisplayTimespanInMillisecond);

    const QVector<DATA_TYPE> & GenerateDAC1();
    const QVector<DATA_TYPE> & GenerateDAC2();
    const QVector<DATA_TYPE> & GenerateDAC3();

    const QVector<DATA_TYPE> & GenerateGate1(int iGateYValue, int iGateBeginInMillisecond, int iGateEndInMillisecond);
    const QVector<DATA_TYPE> & GenerateGate2(int iGateYValue, int iGateBeginInMillisecond, int iGateEndInMillisecond);

    const QVector<DATA_TYPE> & GeneratePlotForTesting();
};

#endif // DATASOURCEPROVIDER_H
