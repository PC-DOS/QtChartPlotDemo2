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

class LineChartLayer{
public:
    LineChartLayer();
    LineChartLayer(QPen qpnPenInit);

    QVector<int> arrDataBuffer; //Access this member directly to set the data of the layer.

    double dXAxisClippingBeginPercentage;
    double dXAxisClippingEndPercentage; //Clipping

    bool bIsVisible; //Indicate if this layer is visible.
    bool bIsCachingDisabled; //Enable or disable caching.
    bool bIsCached; //Indicate if plot data has been cached.
    bool bIsUpdateRequested; //Indicate if user has requested an update.
    bool bIsForcedUpdateRequested; //Indicate if it's necessary to update this layer, including the cache. For example, a size change.

    QPen qpnLinePlotPen; //The pen used to draw the line plot.

    QPainterPath cchPathCache; //Cache

};

class LineChart : public QWidget {
    Q_OBJECT

public:
    LineChart(QWidget *parent = 0);
    ~LineChart();
    virtual void paintEvent(QPaintEvent *); //MAIN FUNTION: We draw our charts in this function.
    virtual void resizeEvent(); //Force an update if size changed.

    QVector<LineChartLayer> Layers; //Access this member directly to manipulate the layers of the chart.

    void AddLayer(QPen qpnLinePlotPenInit=QPen(QColor(0,175,245,255),1)); //Add a layer
    void RemoveLayer(int iLayerIndex); //Remove a layer
    int GetLayerCount() { return Layers.count(); } //Report layer count

    void ReplotSingleLayer(int iLayerIndex=0, bool bUseQueuedReplot=false); //Replot only one layer of the chart.
    Q_SLOT void Replot(bool bUseQueuedReplot=false); //Replot all the charts.

    void SetYAxisRange(int iMinNew=0, int iMaxNew=100); //Set the range of Y axis
    void SetLinePlotPen(int iLayerIndex, QPen qpnPenNew); //Set pen used to plot the data.
    void SetAxisPen(QPen qpnPenNew) { qpnAxisPen=qpnPenNew; } //Set pen used to draw the axis.
    void SetGridPen(QPen qpnPenNew) { qpnGridPen=qpnPenNew; } //Set pen used to draw the background grids.
    void SetMargin(int iLeftNew=20, int iRightNew=20, int iTopNew=20, int iBottomNew=20); //Set the margin of the displayed chart.
    void SetInversedPlotSequence(bool bIsPlotSequenceInversedNew=false) {bIsPlotSequenceInversed=bIsPlotSequenceInversedNew;} //Set if the plotting sequence is inversed. By default, the Widget will plot layers from LayerID 0, but ifIsPlotSequenceInversed is set to True, the plot will start from the layer with the max LayerID.

private:
    QPen qpnAxisPen; //INTERNAL: The pen used to draw the axis.
    QPen qpnGridPen; //INTERNAL: The pen used to draw the background grids.
    int iYAxisMin; //INTERNAL: The minial value of Y axis
    int iYAxisMax; //INTERNAL: The maximum value of Y axis

    int iLeftMargin; //INTERNAL: Margins
    int iRightMargin;
    int iTopMargin;
    int iBottomMargin;

    bool bIsReplotting; //INTERNAL: Check whether the chart is replotting.
    bool bIsReplotQueued; //INTERNAL: Check whether a replot request is queued.
    bool bIsReplotIndexesDefined; //INTERNAL: Indicates if a replotting index as been set.
    bool bIsPlotSequenceInversed; //INTERNAL: Indicates if an inversed plotting sequence is set.
};

#endif // LINECHART_H
