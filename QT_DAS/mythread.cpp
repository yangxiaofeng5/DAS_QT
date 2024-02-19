#include "mythread.h"
#include <pcie_api/ald8xxapi.h>
#include <data_config.h>
#include <QDebug>
#include <QElapsedTimer>
#include "ui_mainwindow.h"

struct user_data_list user_data;

char receive_data_buffer2[1024];

GetSampleData::GetSampleData(QObject *parent)
    : QThread{parent}
{

}

void GetSampleData::recdata(int flag)
{
    thread_flag = flag;
}
static int  data_count= 0;
void api_recv_callback(const char* buffer, int length,const void *pContent)//数据接收回调函数,
{
    receive_data_buffer2[data_count] = *buffer;
    data_count++;
}
void GetSampleData::run()
{
    ui->
    qDebug() << "当前线程对象的地址: " << QThread::currentThread();
    QElapsedTimer time; //计算程序运行时间
    QVector<int> list_ch1;
    QVector<int> list_ch2;

    int ret = start_sample(api_recv_callback,&user_data);
    int data_read_count = 0;
    int milsec;//运行时间计时
    while (thread_flag)
    {
        time.start();
        for(data_read_count = 0;data_read_count<sizeof(receive_data_buffer2);data_read_count++)
        {
            //数据摆放方式为 ch1 ch2 ch1 ch2
            if(data_read_count%2 == 0)
            {
                list_ch2.push_back((int)receive_data_buffer2[data_read_count]);
            }
            else
            {
                list_ch1.push_back((int)receive_data_buffer2[data_read_count]);
            }
        }
        milsec = time.elapsed();
        qDebug()<< "读取数据时间"<<milsec<<"milsec";
        QThread::usleep(1);
        emit sendArry(list_ch1,list_ch2);
    }
    stop_sample();
    qDebug() << "run() 执行完毕, 子线程退出...";
}
