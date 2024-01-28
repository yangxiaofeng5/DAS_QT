#pragma once

#include <QObject>
#include <qcustomplot/qcustomplot.h>
#include<QTimer>
#include<QQueue>
class DynamicDraw : public QObject
{
    Q_OBJECT

public:
    DynamicDraw(QCustomPlot* customPlot, QObject* parent = nullptr);

    ~DynamicDraw();
    void allotBuffer(u_short size, float initialValue = 0);
    void addData(float data);
    void startDraw(u_short lineRefreshTime = 30, u_short axisRefreshTime = 100);
    void magnify();
    void shrink();
private slots:
    void on_refreshLine_timeOut();
    void on_refreshAxis_timeOut();
private:
    QCustomPlot* m_draw;
    QTimer m_lineTimer;
    QTimer m_axisTimer;
    QQueue<float> m_dataQueue;
    u_short m_bufferSize;
    int m_max;
    int m_min;
    float m_magnification;
    QCPGraphDataContainer* m_drawBuffer;
};

