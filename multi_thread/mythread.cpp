#include "mythread.h"
#include <QDebug>
#include <pcie_api/ald8xxapi.h>
#include <QVector>
#include <QFile>

QVector<char> g_best;
//QVector<int> ch1_get_data;
//QVector<int> ch2_get_data;

static int tid_receive_quit = 0;
MyThread::MyThread(QObject *parent) : QThread(parent)
{

}

static int show_count = 0;
struct read_data_list user_data;
void api_recv_callback(const char* buffer, int length,const void *pContent)//数据接收回调函数,
{

    if (show_count <= 4095)//速度比较快，避免控制台显示太多
    {
        qDebug()<<"api_recv_callback,length"<<buffer<<"show_count,"<<show_count;
        //receive_data_buffer2[show_count] = *buffer;
        g_best.append(*buffer);
    }
    show_count++;
}

void MyThread::run()
{
    qDebug() << "当前线程对象的地址: " << QThread::currentThread();

    int num = 0;
    int ret = start_sample(api_recv_callback,&user_data);
    qDebug()<<"startsample"<< ret;
    while(!tid_receive_quit)
    {
        emit curNumber(num++);
        if(num == 500)
        {
            break;
        }
        QThread::usleep(1);
    }
    stop_sample();
    qDebug() << "run() 执行完毕, 子线程退出..."<<"采集数据长度为"<<g_best.size();

    //保存为csv数据
    QFile file_ch1("D://Program_Station/QT_Program/QT_Thread/multi_thread/data_save/out_ch1.csv");
    QFile file_ch2("D://Program_Station/QT_Program/QT_Thread/multi_thread/data_save/out_ch2.csv");
    if(file_ch1.open(QIODevice::WriteOnly|QIODevice::Text)&&file_ch2.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream out_ch1(&file_ch1);
        QTextStream out_ch2(&file_ch2);
        for(int i=0;i<g_best.size();i+=4){
            out_ch1<<(int)g_best.at(i+1);
            out_ch1<<"\n";
            out_ch2<<(int)g_best.at(i+3);
            out_ch2<<"\n";
        }
        file_ch1.close();
        file_ch2.close();
        qDebug() <<"file write true";
    }
    else{
        qDebug() <<"file write false";
    }
}
