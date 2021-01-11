#include "LineChart.h"

LineChart::LineChart(QWidget *parent){
    Layers.push_back(LineChartLayer());
    _AxisPen=QPen(Qt::white,1.5);
    _GridPen=QPen(Qt::white,0.5,Qt::DashLine);
    _iYAxisMin=0;
    _iYAxisMax=50;
    _iLeftMargin=20;
    _iRightMargin=20;
    _iTopMargin=20;
    _iBottomMargin=20;
    _IsReplotting=false;
    _IsReplotQueued=false;
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

    if (0==Layers.count()){
        return;
    }

    for (int iLayerIndex=0; iLayerIndex<Layers.count(); ++iLayerIndex){
        if (0==Layers[iLayerIndex].DataBuffer.count()){
            continue;
        }
        painter.setPen(Layers[iLayerIndex].LinePlotPen);
        int chartWidth = this->width()-_iLeftMargin-_iRightMargin;
        int chartHeight = this->height()-_iTopMargin-_iBottomMargin;
        int yMax = _iYAxisMax-_iYAxisMin;
        int xMax = Layers[iLayerIndex].DataBuffer.count();
        if (!Layers[iLayerIndex].IsCachingDisabled && !Layers[iLayerIndex].IsUpdateRequested && !Layers[iLayerIndex].IsForcedUpdateRequested && Layers[iLayerIndex].IsCached){ //Load buffer for unchanged layers
            painter.drawPath(Layers[iLayerIndex]._PathCache);
            continue;
        }
        QPainterPath path;
        QPointF tmp;
        tmp.setX(_iLeftMargin);
        tmp.setY(_iTopMargin+chartHeight*(yMax-Layers[iLayerIndex].DataBuffer[0])/yMax);
        path.moveTo(tmp);
        for (int i = 1; i < xMax-1; i++){
            tmp.setX(_iLeftMargin+chartWidth*i/xMax);
            tmp.setY(_iTopMargin+chartHeight*(yMax-Layers[iLayerIndex].DataBuffer[i])/yMax);
            path.lineTo(tmp);
        }
        painter.drawPath(path);

        if (!Layers[iLayerIndex].IsCachingDisabled){
            Layers[iLayerIndex]._PathCache=path;
        }
    }
}

void LineChart::ReplotSingleLayer(int iLayerIndex, bool bUseQueuedReplot){
    _IsReplotIndexesDefined=false;
    for (int i=0;i<Layers.count();++i){
        if (i==iLayerIndex){
            Layers[i].IsUpdateRequested=true;
        }
        else{
            Layers[i].IsUpdateRequested=false;
        }
    }
    _IsReplotIndexesDefined=true;

    if (bUseQueuedReplot){ //Queue a replot request
        if (!_IsReplotQueued){
            _IsReplotQueued=true;
            QTimer::singleShot(0, this, SLOT(Replot()));
        }
    }

    if (_IsReplotting){
        return; //Avoid signal loopback
    }

    _IsReplotting=true;
    _IsReplotQueued=false;

    this->update();

    _IsReplotting=false;
}

void LineChart::Replot(bool bUseQueuedReplot){
    if (!_IsReplotIndexesDefined){
        for (int i=0;i<Layers.count();++i){
            Layers[i].IsUpdateRequested=true;
        }
    }

    if (bUseQueuedReplot){ //Queue a replot request
        if (!_IsReplotQueued){
            _IsReplotQueued=true;
            QTimer::singleShot(0, this, SLOT(Replot()));
        }
    }

    if (_IsReplotting){
        return; //Avoid signal loopback
    }

    _IsReplotting=true;
    _IsReplotQueued=false;

    this->update();

    _IsReplotting=false;
}

void LineChart::AddLayer(QPen qpnLinePlotPen){
    Layers.push_back(LineChartLayer(qpnLinePlotPen));
}

void LineChart::RemoveLayer(int iLayerIndex){
    if (iLayerIndex>=0 && iLayerIndex<Layers.count()){
        Layers.remove(iLayerIndex);
    }
}

void LineChart::SetYAxisRange(int iMin, int iMax){
    if (iMax<=iMin){
        iMax=iMin+1;
    }
    _iYAxisMin=iMin;
    _iYAxisMax=iMax;
}

void LineChart::SetLinePlotPen(int iLayerIndex, QPen qpnPen){
    if (iLayerIndex>=0 && iLayerIndex<Layers.count()){
        Layers[iLayerIndex].LinePlotPen=qpnPen;
    }
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

LineChartLayer::LineChartLayer(){
    LinePlotPen=QPen(QColor(0,175,245,255),1);
    IsCachingDisabled=false;
    IsCached=false;
    IsUpdateRequested=false;
    IsForcedUpdateRequested=true;
}

LineChartLayer::LineChartLayer(QPen qpnPen){
    LinePlotPen=qpnPen;
    IsCachingDisabled=false;
    IsCached=false;
    IsUpdateRequested=false;
    IsForcedUpdateRequested=true;
}
