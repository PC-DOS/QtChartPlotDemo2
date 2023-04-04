#include "LineChart.h"

LineChart::LineChart(QWidget *parent){
    setAutoFillBackground(true);
    Layers.push_back(LineChartLayer());
    qpnAxisPen=QPen(Qt::white,1.5);
    qpnGridPen=QPen(Qt::white,0.5,Qt::DashLine);
    iYAxisMin=0;
    iYAxisMax=50;
    iLeftMargin=20;
    iRightMargin=20;
    iTopMargin=20;
    iBottomMargin=20;
    bIsReplotting=false;
    bIsReplotQueued=false;
    bIsPlotSequenceInversed=false;
}

LineChart::~LineChart(){

}

void LineChart::paintEvent(QPaintEvent *){

    /* Calculate canvas */
    QPoint TopLeft(iLeftMargin, iTopMargin);
    QPoint TopRight(this->width()-iRightMargin, iTopMargin);
    QPoint BottomLeft(iBottomMargin, this->height()-iLeftMargin);
    QPoint BottomRight(this->width()-iRightMargin, this->height()-iBottomMargin);

    QPainter painter(this);

    /* Draw axis */
    painter.setPen(qpnAxisPen);
    painter.drawLine(TopLeft, BottomLeft);
    painter.drawLine(BottomLeft, BottomRight);
    painter.drawLine(TopLeft,TopRight);
    painter.drawLine(TopRight,BottomRight);

    /* Draw grid */
    painter.setPen(qpnGridPen);
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

    /* Check if there is no layers */
    if (0==Layers.count()){
        return;
    }

    /* Draw every layer */
    for (int iLayerIndexFake=0; iLayerIndexFake<Layers.count(); ++iLayerIndexFake){
        int iLayerIndex = bIsPlotSequenceInversed?Layers.count()-1-iLayerIndexFake:iLayerIndexFake; //Calculate the real layer index.
        if (0==Layers.at(iLayerIndex).arrDataBuffer.count()){ //Check if we have data.
            continue;
        }
        if (!Layers.at(iLayerIndex).bIsVisible){ //Skip invisible layers.
            continue;
        }
        painter.setPen(Layers.at(iLayerIndex).qpnLinePlotPen);
        int chartWidth = this->width()-iLeftMargin-iRightMargin;
        int chartHeight = this->height()-iTopMargin-iBottomMargin;
        int yMax = iYAxisMax-iYAxisMin;
        int xMax = Layers.at(iLayerIndex).arrDataBuffer.count();
        if (!Layers.at(iLayerIndex).bIsCachingDisabled && !Layers.at(iLayerIndex).bIsUpdateRequested && !Layers.at(iLayerIndex).bIsForcedUpdateRequested && Layers.at(iLayerIndex).bIsCached){ //Load buffer for unchanged layers
            painter.drawPath(Layers.at(iLayerIndex).cchPathCache); //If an up-to-date cache is available, draw it.
            continue;
        }
        QPainterPath path;
        QPointF tmp;
        tmp.setX(iLeftMargin+chartWidth*Layers.at(iLayerIndex).dXAxisClippingBeginPercentage);
        tmp.setY(iTopMargin+chartHeight*(yMax-Layers.at(iLayerIndex).arrDataBuffer.at(Layers.at(iLayerIndex).arrDataBuffer.count()*Layers.at(iLayerIndex).dXAxisClippingBeginPercentage))/yMax);
        path.moveTo(tmp);
        for (int i = Layers.at(iLayerIndex).arrDataBuffer.count()*Layers.at(iLayerIndex).dXAxisClippingBeginPercentage; i < xMax*Layers.at(iLayerIndex).dXAxisClippingEndPercentage; ++i){
            tmp.setX(iLeftMargin+chartWidth*i/xMax);
            tmp.setY(iTopMargin+chartHeight*(yMax-Layers.at(iLayerIndex).arrDataBuffer.at(i))/yMax);
            path.lineTo(tmp);
        }
        painter.drawPath(path);

        if (!Layers.at(iLayerIndex).bIsCachingDisabled){ //Save path to cache
            Layers[iLayerIndex].cchPathCache=path;
            Layers[iLayerIndex].bIsCached=true;
        }

        Layers[iLayerIndex].bIsForcedUpdateRequested=false;
    }
}

void LineChart::resizeEvent(){ //If size changed, force a full update
    for (int i=0; i<Layers.count(); ++i){
        Layers[i].bIsForcedUpdateRequested=true;
    }
}

/*
 * Replot a single layer, specify the index of the layer in iLayerIndex
 *
 * Set bUseQueuedReplot to true, and the replot will be queued in the next event loop. Use this for better performance.
 * Ref. QCustomPlot
 */
void LineChart::ReplotSingleLayer(int iLayerIndex, bool bUseQueuedReplot){
    bIsReplotIndexesDefined=false;
    for (int i=0;i<Layers.count();++i){
        if (i==iLayerIndex){
            Layers[i].bIsUpdateRequested=true;
        }
        else{
            Layers[i].bIsUpdateRequested=false;
        }
    }
    bIsReplotIndexesDefined=true;

    if (bUseQueuedReplot){ //Queue a replot request
        if (!bIsReplotQueued){
            bIsReplotQueued=true;
            QTimer::singleShot(0, this, SLOT(Replot()));
        }
    }

    if (bIsReplotting){
        return; //Avoid signal loopback
    }

    bIsReplotting=true;
    bIsReplotQueued=false;

    this->update();

    bIsReplotting=false;

    bIsReplotIndexesDefined=false;
}

void LineChart::Replot(bool bUseQueuedReplot){
    if (!bIsReplotIndexesDefined){
        for (int i=0;i<Layers.count();++i){
            Layers[i].bIsUpdateRequested=true;
        }
    }

    if (bUseQueuedReplot){ //Queue a replot request
        if (!bIsReplotQueued){
            bIsReplotQueued=true;
            QTimer::singleShot(0, this, SLOT(Replot()));
        }
    }

    if (bIsReplotting){
        return; //Avoid signal loopback
    }

    bIsReplotting=true;
    bIsReplotQueued=false;

    this->update();

    bIsReplotting=false;
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
    iYAxisMin=iMin;
    iYAxisMax=iMax;
}

void LineChart::SetLinePlotPen(int iLayerIndex, QPen qpnPen){
    if (iLayerIndex>=0 && iLayerIndex<Layers.count()){
        Layers[iLayerIndex].qpnLinePlotPen=qpnPen;
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
    iLeftMargin=iLeft;
    iRightMargin=iRight;
    iTopMargin=iTop;
    iBottomMargin=iBottom;
}

LineChartLayer::LineChartLayer(){
    qpnLinePlotPen=QPen(QColor(0,175,245,255),1);
    bIsVisible=true;
    bIsCachingDisabled=false;
    bIsCached=false;
    bIsUpdateRequested=true;
    bIsForcedUpdateRequested=true;
    dXAxisClippingBeginPercentage=0;
    dXAxisClippingEndPercentage=1;
}

LineChartLayer::LineChartLayer(QPen qpnPenInit){
    qpnLinePlotPen=qpnPenInit;
    bIsVisible=true;
    bIsCachingDisabled=false;
    bIsCached=false;
    bIsUpdateRequested=true;
    bIsForcedUpdateRequested=true;
    dXAxisClippingBeginPercentage=0;
    dXAxisClippingEndPercentage=1;
}
