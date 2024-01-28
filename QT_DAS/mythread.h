#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QVector>

class GetSampleData : public QThread
{
    Q_OBJECT
public:
    explicit GetSampleData(QObject *parent = nullptr);

    void recdata(int thread_flag);

protected:
    void run() override;

signals:
    void sendArry(QVector<int> data_list_ch1,QVector<int> data_list_ch2);

private:
    int thread_flag;
};

#endif // MYTHREAD_H
