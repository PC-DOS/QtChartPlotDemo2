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

class DataSourceProvider{
private:
    int _iCurrentSamplingRate;
    double _dCurrentGain; //In multiples, to generate value in dB, use: dB = 20lg(Multiples)
    int _iCurrentDisplayTimespan;
    int _iPointsPerPlot;

    QVector<double> _arrData;

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

    QString SamplingRateToString(bool IsUnitTranslationEnabled=true);
    QString SamplingRateToString(int iSamplingRateInHz, bool IsUnitTranslationEnabled=true);
    QString GainToString(bool IsDbEnabled=true, bool IsDbOnly=false);
    QString GainToString(double dGainInMultiple, bool IsDbEnabled=true, bool IsDbOnly=false);
    QString DisplayTimespanToString();
    QString DisplayTimespanToString(int iDisplayTimespanInMillisecond);

    const QVector<double> &GeneratePlotForTesting();
};

#endif // DATASOURCEPROVIDER_H
