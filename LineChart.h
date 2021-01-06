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

    QVector<double> DataBuffer;

    void Replot();

    void SetYAxisRange(int iMin=0, int iMax=100);
    void SetLinePlotPen(QPen qpnPen) { _LinePlotPen=qpnPen; }
    void SetAxisPen(QPen qpnPen) { _AxisPen=qpnPen; }
    void SetSplitterPen(QPen qpnPen) { _SplitterPen=qpnPen; }
    void SetMargin(int iLeft=20, int iRight=20, int iTop=20, int iBottom=20);

private:
    QPen _LinePlotPen;
    QPen _AxisPen;
    QPen _SplitterPen;
    int _iYAxisMin;
    int _iYAxisMax;

    int _iLeftMargin;
    int _iRightMargin;
    int _iTopMargin;
    int _iBottomMargin;
};

#endif // LINECHART_H
