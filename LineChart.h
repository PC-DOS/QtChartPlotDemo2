/* LineChart Widget for Qt
 * By Picsell Dois @ Southeast University
 *
 * A simple charting widget which can be used to plot linear charts.
 * Such as a simple function, realtime data, etc.
 *
 * Code Referenced:
 * QCustomPlot: https://www.qcustomplot.com/
 * waveChart: https://github.com/fuchiao/waveChart
 */

#ifndef LINECHART_H
#define LINECHART_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QPixmap>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMultiMap>
#include <QtCore/QFlags>
#include <QtCore/QDebug>
#include <QtCore/QStack>
#include <QtCore/QCache>
#include <QtCore/QMargins>
#include <qmath.h>
#include <limits>
#include <algorithm>
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#  include <qnumeric.h>
#  include <QtGui/QWidget>
#  include <QtGui/QPrinter>
#  include <QtGui/QPrintEngine>
#else
#  include <QtNumeric>
#  include <QtWidgets/QWidget>
#  include <QtPrintSupport/QtPrintSupport>
#endif
#include <QPen>
#include <QPainter>

class LineChart : public QWidget {
    Q_OBJECT

public:
    LineChart(QWidget *parent = 0);
    ~LineChart();
    virtual void paintEvent(QPaintEvent *);

    QVector<double> DataBuffer; //Access this member directly to set the data of the chart.

    Q_SLOT void Replot(bool bUseQueuedReplot=false); //Replot the chart.

    void SetYAxisRange(int iMin=0, int iMax=100); //Set the range of Y axis
    void SetLinePlotPen(QPen qpnPen) { _LinePlotPen=qpnPen; } //Set pen used to plot the data.
    void SetAxisPen(QPen qpnPen) { _AxisPen=qpnPen; } //Set pen used to draw the axis.
    void SetGridPen(QPen qpnPen) { _GridPen=qpnPen; } //Set pen used to draw the background grids.
    void SetMargin(int iLeft=20, int iRight=20, int iTop=20, int iBottom=20); //Set the margin of the displayed chart.

private:
    QPen _LinePlotPen; //INTERNAL: The pen used to plot the data.
    QPen _AxisPen; //INTERNAL: The pen used to draw the axis.
    QPen _GridPen; //INTERNAL: The pen used to draw the background grids.
    int _iYAxisMin; //INTERNAL: The minial value of Y axis
    int _iYAxisMax; //INTERNAL: The maximum value of Y axis

    int _iLeftMargin; //INTERNAL: Margins
    int _iRightMargin;
    int _iTopMargin;
    int _iBottomMargin;

    bool _IsReplotting; //INTERNAL: Check whether the chart is replotting.
    bool _IsReplotQueued; //INTERNAL: Check whether a replot request is queued.
};

#endif // LINECHART_H
