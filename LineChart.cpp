#include "LineChart.h"

LineChart::LineChart(QWidget *parent){
    _AxisPen=QPen(Qt::white,1.5);
    _GridPen=QPen(Qt::white,0.5,Qt::DashLine);
    _LinePlotPen=QPen(QColor(0,175,245,255),1);
    _iYAxisMin=0;
    _iYAxisMax=50;
    _iLeftMargin=20;
    _iRightMargin=20;
    _iTopMargin=20;
    _iBottomMargin=20;
}

LineChart::~LineChart(){

}

void LineChart::paintEvent(QPaintEvent *){
    QPoint TopLeft(_iLeftMargin, _iTopMargin);
    QPoint TopRight(this->width()-_iRightMargin, _iTopMargin);
    QPoint BottomLeft(_iBottomMargin, this->height()-_iLeftMargin);
    QPoint BottomRight(this->width()-_iRightMargin, this->height()-_iBottomMargin);

    QPainter painter(this);
    painter.setPen(_AxisPen);
    painter.drawLine(TopLeft, BottomLeft);
    painter.drawLine(BottomLeft, BottomRight);
    painter.drawLine(TopLeft,TopRight);
    painter.drawLine(TopRight,BottomRight);

    painter.setPen(_GridPen);
    if (this->width()>200)
    {
        painter.drawLine((TopLeft+TopRight)/2, (BottomLeft+BottomRight)/2);
        if (this->width()>400)
        {
            painter.drawLine(TopLeft+(TopRight-TopLeft)/4, BottomLeft+(BottomRight-BottomLeft)/4);
            painter.drawLine(TopLeft+(TopRight-TopLeft)*3/4, BottomLeft+(BottomRight-BottomLeft)*3/4);
        }
    }
    if (this->height()>125)
    {
        painter.drawLine((TopLeft+BottomLeft)/2, (TopRight+BottomRight)/2);
        if (this->height()>245)
        {
            painter.drawLine(TopLeft+(BottomLeft-TopLeft)/4, TopRight+(BottomRight-TopRight)/4);
            painter.drawLine(TopLeft+(BottomLeft-TopLeft)*3/4, TopRight+(BottomRight-TopRight)*3/4);
        }
    }

    if (0==DataBuffer.size()){
        return;
    }

    painter.setPen(_LinePlotPen);
    int chartWidth = this->width()-_iLeftMargin-_iRightMargin;
    int chartHeight = this->height()-_iTopMargin-_iBottomMargin;
    int yMax = _iYAxisMax;
    int xMax = DataBuffer.count();
    QPainterPath path;
    QPointF tmp;
    tmp.setX(_iLeftMargin);
    tmp.setY(_iTopMargin+chartHeight*(yMax-DataBuffer[0])/yMax);
    path.moveTo(tmp);
    for (int i = 1; i < xMax-1; i++){
        tmp.setX(_iLeftMargin+chartWidth*i/xMax);
        tmp.setY(_iTopMargin+chartHeight*(yMax-DataBuffer[i])/yMax);
        path.lineTo(tmp);
    }
    painter.drawPath(path);
}

void LineChart::Replot(){
    this->update();
}

void LineChart::SetYAxisRange(int iMin, int iMax){
    if (iMax<=iMin){
        iMax=iMin+1;
    }
    _iYAxisMin=iMin;
    _iYAxisMax=iMax;
}

void LineChart::SetMargin(int iLeft, int iRight, int iTop, int iBottom){
    if (iLeft<0){
        iLeft=0;
    }
    if (iRight<0){
        iRight=0;
    }
    if (iTop<0){
        iTop=0;
    }
    if (iBottom<0){
        iBottom=0;
    }
    _iLeftMargin=iLeft;
    _iRightMargin=iRight;
    _iTopMargin=iTop;
    _iBottomMargin=iBottom;
}
