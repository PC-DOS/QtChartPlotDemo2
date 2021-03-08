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
    LineChartLayer(QPen qpnPen);

    QVector<int> DataBuffer; //Access this member directly to set the data of the layer.

    double XAxisClippingBeginPercentage;
    double XAxisClippingEndPercentage; //Clipping

    bool IsVisible; //Indicate if this layer is visible.
    bool IsCachingDisabled; //Enable or disable caching.
    bool IsCached; //Indicate if plot data has been cached.
    bool IsUpdateRequested; //Indicate if user has requested an update.
    bool IsForcedUpdateRequested; //Indicate if it's necessary to update this layer, including the cache. For example, a size change.

    QPen LinePlotPen; //The pen used to draw the line plot.

    //INTERNAL MEMBERS//
    QPainterPath _PathCache; //Cache

};

class LineChart : public QWidget {
    Q_OBJECT

public:
    LineChart(QWidget *parent = 0);
    ~LineChart();
    virtual void paintEvent(QPaintEvent *); //MAIN FUNTION: We draw our charts in this function.
    virtual void resizeEvent(); //Force an update if size changed.

    QVector<LineChartLayer> Layers; //Access this member directly to manipulate the layers of the chart.

    void AddLayer(QPen qpnLinePlotPen=QPen(QColor(0,175,245,255),1)); //Add a layer
    void RemoveLayer(int iLayerIndex); //Remove a layer
    int GetLayerCount() { return Layers.count(); } //Report layer count

    void ReplotSingleLayer(int iLayerIndex=0, bool bUseQueuedReplot=false); //Replot only one layer of the chart.
    Q_SLOT void Replot(bool bUseQueuedReplot=false); //Replot all the charts.

    void SetYAxisRange(int iMin=0, int iMax=100); //Set the range of Y axis
    void SetLinePlotPen(int iLayerIndex, QPen qpnPen); //Set pen used to plot the data.
    void SetAxisPen(QPen qpnPen) { _AxisPen=qpnPen; } //Set pen used to draw the axis.
    void SetGridPen(QPen qpnPen) { _GridPen=qpnPen; } //Set pen used to draw the background grids.
    void SetMargin(int iLeft=20, int iRight=20, int iTop=20, int iBottom=20); //Set the margin of the displayed chart.
    void SetInversedPlotSequence(bool IsPlotSequenceInversed=false) {_IsPlotSequenceInversed=IsPlotSequenceInversed;} //Set if the plotting sequence is inversed. By default, the Widget will plot layers from LayerID 0, but ifIsPlotSequenceInversed is set to True, the plot will start from the layer with the max LayerID.

private:
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
    bool _IsReplotIndexesDefined; //INTERNAL: Indicates if a replotting index as been set.
    bool _IsPlotSequenceInversed; //INTERNAL: Indicates if an inversed plotting sequence is set.
};

#endif // LINECHART_H
