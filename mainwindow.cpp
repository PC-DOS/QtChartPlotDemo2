#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "StateMachine.h"
#include "DataSourceProvider.h"
#include <QVector>
#include <QString>
#include <QStringBuilder>
#include <QTimer>
#include <math.h>

DataSourceProvider * datUltrasoud;
StateMachine * smCurrentState;
QTimer *tmrDataGenerationTimer;

QVector<double> arrXAxis;

#ifdef TIMER_PERFORMANCE_TESTING
QTimer *tmrFrameCounter;
unsigned long long iTotalFrames=0;
unsigned int iCurrentFrames=0;
unsigned int iFps=1;
#endif

void MainWindow::RegenerateXAxisData(){
    arrXAxis.clear();
    for (int i=0; i<datUltrasoud->GetCurrentPointsPerPlot();++i){
        arrXAxis.push_back(double(i)*double(1000)/double(datUltrasoud->GetCurrentSamlingRate()));
    }
    ui->chrtData->DataBuffer[1]=datUltrasoud->GenerateDAC1();
    ui->chrtData->DataBuffer[2]=datUltrasoud->GenerateDAC2();
    ui->chrtData->DataBuffer[3]=datUltrasoud->GenerateDAC3();
    ui->chrtData->Replot(true);
}

void MainWindow::UpdateAxisData(){

}

void MainWindow::tmrDataGenerationTimer_Tick(){
    ui->chrtData->DataBuffer[0]=datUltrasoud->GeneratePlotForTesting();
    ui->chrtData->ReplotSingleLayer(0,true);
#ifdef TIMER_PERFORMANCE_TESTING
    ++iCurrentFrames;
    //++iTotalFrames;
    //ui->lblCounter->setText(QString::number(iTotalFrames)+QString(" Plot(s)")+QString("\r\n")+QString::number(iFps)+QString(" fps"));
    //ui->lblCounter->setText(QString::number(iFps)+QString(" fps"));
#endif
    //qApp->processEvents();
    //QApplication::processEvents();
    return;
}

#ifdef TIMER_PERFORMANCE_TESTING
void MainWindow::tmrFrameCounter_Tick(){
    iFps=iCurrentFrames;
    iCurrentFrames=0;
    //ui->lblCounter->setText(QString::number(iTotalFrames)+QString(" Plot(s)")+QString("\r\n")+QString::number(iFps)+QString(" fps"));
    ui->lblCounter->setText(QString::number(iFps)+QString(" fps"));
    return;
}
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set fake battery percentage
    ui->prgBattPct->setValue(100);

    //Initialize DataSourceProvider
    datUltrasoud=new DataSourceProvider();
    smCurrentState=new StateMachine();
    ui->lblSamplingRate->setText(datUltrasoud->SamplingRateToString());
    ui->lblGain->setText(datUltrasoud->GainToString());
    ui->lblTimespan->setText(datUltrasoud->DisplayTimespanToString());

    //Chart Widget Initialization
    ui->chrtData->SetYAxisRange(0,25);
    ui->chrtData->AddLayer(QPen(Qt::red,1));
    ui->chrtData->AddLayer(QPen(Qt::red,1));
    ui->chrtData->AddLayer(QPen(Qt::red,1));

    //Start Timer
    RegenerateXAxisData();
    tmrDataGenerationTimer = new QTimer(this);
    connect(tmrDataGenerationTimer, SIGNAL(timeout()), this, SLOT(tmrDataGenerationTimer_Tick()));
    tmrDataGenerationTimer->start(datUltrasoud->GetCurrentDisplayTimespan());

#ifdef TIMER_PERFORMANCE_TESTING
    //Frame Counter
    //ui->lblCounter->setText(QString::number(iTotalFrames)+QString(" Plot(s)")+QString("\r\n")+QString::number(iFps)+QString(" fps"));
    ui->lblCounter->setText(QString::number(iFps)+QString(" fps"));
    tmrFrameCounter=new QTimer(this);
    connect(tmrFrameCounter, SIGNAL(timeout()), this, SLOT(tmrFrameCounter_Tick()));
    tmrFrameCounter->start(1000);
#else
    ui->lblCounter->setVisible(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *KeyEventArgs){
    QWidget::keyPressEvent(KeyEventArgs);
}

void MainWindow::keyReleaseEvent(QKeyEvent *KeyEventArgs){
    QPalette pltSamplingRate(ui->wgtSamplingRate->palette());
    QPalette pltGain(ui->wgtGain->palette());
    QPalette pltTimespan(ui->wgtTimespan->palette());
    switch (KeyEventArgs->key()){
    case Qt::Key_1: //Finish
        smCurrentState->SetCurrentState(SM_STATE_NOTMAL);
        pltSamplingRate.setColor(QPalette::Background,QColor(75,75,75));
        pltGain.setColor(QPalette::Background,QColor(75,75,75));
        pltTimespan.setColor(QPalette::Background,QColor(75,75,75));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_2: //Value plus
        switch (smCurrentState->GetCurrentState()){
        case SM_STATE_NOTMAL:
            break;
        case SM_STATE_SAMPLINGRATE_ADJUSTING:
            if (datUltrasoud->GetCurrentSamlingRate()<=__INT_MAX__){
                datUltrasoud->SetCurrentSamplingRate(datUltrasoud->GetCurrentSamlingRate()*2);
                ui->lblSamplingRate->setText(datUltrasoud->SamplingRateToString());
                RegenerateXAxisData();
                UpdateAxisData();
            }
            break;
        case SM_STATE_GAIN_ADJUSTING:
            if (datUltrasoud->GetCurrentGainInMultiple()<=4){
                datUltrasoud->SetCurrentGainInMultiple(datUltrasoud->GetCurrentGainInMultiple()*2);
                ui->lblGain->setText(datUltrasoud->GainToString());
            }
            break;
        case SM_STATE_TIMESPAN_ADJUSTING:
            if (datUltrasoud->GetCurrentDisplayTimespan()<=__INT_MAX__){
                datUltrasoud->SetCurrentDisplayTimespan(datUltrasoud->GetCurrentDisplayTimespan()*2);
                ui->lblTimespan->setText(datUltrasoud->DisplayTimespanToString());
                RegenerateXAxisData();
                UpdateAxisData();
                tmrDataGenerationTimer->setInterval(datUltrasoud->GetCurrentDisplayTimespan());
            }
            break;
        default:
            break;
        }

        break;
    case Qt::Key_3: //Value minus
        switch (smCurrentState->GetCurrentState()){
        case SM_STATE_NOTMAL:
            break;
        case SM_STATE_SAMPLINGRATE_ADJUSTING:
            if (datUltrasoud->GetCurrentSamlingRate()>=50){
                datUltrasoud->SetCurrentSamplingRate(datUltrasoud->GetCurrentSamlingRate()/2);
                ui->lblSamplingRate->setText(datUltrasoud->SamplingRateToString());
                RegenerateXAxisData();
                UpdateAxisData();
            }
            break;
        case SM_STATE_GAIN_ADJUSTING:
            if (datUltrasoud->GetCurrentGainInMultiple()>=0.5){
                datUltrasoud->SetCurrentGainInMultiple(datUltrasoud->GetCurrentGainInMultiple()/2);
                ui->lblGain->setText(datUltrasoud->GainToString());
            }
            break;
        case SM_STATE_TIMESPAN_ADJUSTING:
            if (datUltrasoud->GetCurrentDisplayTimespan()>=2){
                datUltrasoud->SetCurrentDisplayTimespan(datUltrasoud->GetCurrentDisplayTimespan()/2);
                ui->lblTimespan->setText(datUltrasoud->DisplayTimespanToString());
                RegenerateXAxisData();
                UpdateAxisData();
                tmrDataGenerationTimer->setInterval(datUltrasoud->GetCurrentDisplayTimespan());
            }
            break;
        default:
            break;
        }

        break;
    case Qt::Key_4: //Set Timespan
        smCurrentState->SetCurrentState(SM_STATE_TIMESPAN_ADJUSTING);
        pltSamplingRate.setColor(QPalette::Background,QColor(75,75,75));
        pltGain.setColor(QPalette::Background,QColor(75,75,75));
        pltTimespan.setColor(QPalette::Background,QColor(150,150,150));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_8: //Set Gain
        smCurrentState->SetCurrentState(SM_STATE_GAIN_ADJUSTING);
        pltSamplingRate.setColor(QPalette::Background,QColor(75,75,75));
        pltGain.setColor(QPalette::Background,QColor(150,150,150));
        pltTimespan.setColor(QPalette::Background,QColor(75,75,75));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_B: //Set Sampling Rate
        smCurrentState->SetCurrentState(SM_STATE_SAMPLINGRATE_ADJUSTING);
        pltSamplingRate.setColor(QPalette::Background,QColor(150,150,150));
        pltGain.setColor(QPalette::Background,QColor(75,75,75));
        pltTimespan.setColor(QPalette::Background,QColor(75,75,75));
        ui->wgtSamplingRate->setPalette(pltSamplingRate);
        ui->wgtGain->setPalette(pltGain);
        ui->wgtTimespan->setPalette(pltTimespan);
        break;
    case Qt::Key_F:

        break;
    default:
        break;
    }
    QWidget::keyReleaseEvent(KeyEventArgs);
}
