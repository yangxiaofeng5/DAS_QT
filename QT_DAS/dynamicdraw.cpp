#include "dynamicdraw.h"

//DynamicDraw::DynamicDraw(QObject *parent)
//    : QObject{parent}
//{
//
//}
//#include "DynamicDraw.h"

DynamicDraw::DynamicDraw(QCustomPlot* customPlot,QObject *parent)
    : QObject(parent),m_draw(customPlot),m_bufferSize(0),m_magnification(1),m_max(0),m_min(0),m_drawBuffer(NULL)
{
    connect(&m_lineTimer, &QTimer::timeout, this, &DynamicDraw::on_refreshLine_timeOut);
    connect(&m_axisTimer, &QTimer::timeout, this, &DynamicDraw::on_refreshAxis_timeOut);
}

DynamicDraw::~DynamicDraw()
{

}

void DynamicDraw::allotBuffer(u_short size, float initialValue)
{
    while (m_draw->removeGraph(0));
    m_bufferSize = size;
    QSharedPointer<QCPGraphDataContainer> dataContainer = m_draw->addGraph()->data();
    QVector<QCPGraphData> plotData(m_bufferSize);
    for (size_t j = 0; j < m_bufferSize; j++)
    {
        plotData[j].key = j;
        plotData[j].value = initialValue;
    }
    dataContainer->set(plotData, true);
    m_draw->xAxis->setRange(0, m_bufferSize);
    m_dataQueue.clear();
    m_drawBuffer = m_draw->graph()->data().data();
}

void DynamicDraw::addData(float data)
{
    if (m_dataQueue.size() >= m_bufferSize)
        m_dataQueue.dequeue();
    m_dataQueue.enqueue(data);
}

void DynamicDraw::startDraw(u_short lineRefreshTime, u_short axisRefreshTime)
{
    if (m_lineTimer.isActive())
        m_lineTimer.stop();
    if (m_axisTimer.isActive())
        m_axisTimer.stop();
    m_lineTimer.start(lineRefreshTime);
    m_axisTimer.start(axisRefreshTime);
}

void DynamicDraw::magnify()
{
    if (m_magnification > 0.1)
        m_magnification -= 0.1;
}

void DynamicDraw::shrink()
{
    if (m_magnification < 5)
        m_magnification += 0.1;
}

void DynamicDraw::on_refreshLine_timeOut()
{
    size_t size = m_dataQueue.size();
    for (size_t j = size - 1; j; --j)
    {
        QCPGraphData* buff = (QCPGraphData*)m_drawBuffer->at(j);
        buff->value = m_dataQueue.at(j);
    }
    m_draw->replot(QCustomPlot::rpQueuedReplot);
}

void DynamicDraw::on_refreshAxis_timeOut()
{
    m_max = *std::max_element(m_dataQueue.begin(), m_dataQueue.end());
    m_min = *std::min_element(m_dataQueue.begin(), m_dataQueue.end());
    m_draw->xAxis->setRange(0, m_bufferSize);
    if (m_max < 0)
        m_draw->yAxis->setRange(m_min + m_min * m_magnification, m_max - m_max * m_magnification);
    else
    {
        if (m_min > 0)
            m_draw->yAxis->setRange(m_min - m_min * m_magnification, m_max + m_max * m_magnification);
        else
            m_draw->yAxis->setRange(m_min + m_min * m_magnification, m_max + m_max * m_magnification);
    }
}

