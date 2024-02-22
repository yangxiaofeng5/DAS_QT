#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "multi_thread_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}

//#include <QtCore>
//#include <stdio.h>
//#include <stdlib.h>
//#include <QDebug>
//#include <QThread>
//
//const int DataSize=10;
//const int BufferSize=5;
//char buffer[BufferSize];
//QSemaphore freeBytes(BufferSize);
//QSemaphore useBytes;
//
//class Producer : public QThread
//{
//public:
//    void run();
//};
//void Producer::run()
//{
//    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//    for(int i=0;i<DataSize;i++)
//    {
//        freeBytes.acquire();
//        buffer[i%BufferSize]="ACGT"[(int)qrand()%4];
//        qDebug()<<QString("producer:%1").arg(buffer[i%BufferSize]);
//        useBytes.release();
//    }
//}
//
//class Customer : public QThread
//{
//public:
//    void run();
//};
//
//void Customer::run()
//{
//    for(int i=0;i<DataSize;i++)
//    {
//        useBytes.acquire();
//        qDebug()<<QString("customer:%1").arg(buffer[i%BufferSize]);
//        freeBytes.release();
//    }
//}
//
//int main(int argc,char* argv[])
//{
//    QCoreApplication app(argc,argv);
//    Producer producer;
//    Customer customer;
//    producer.start();
//    customer.start();
//    producer.wait();
//    customer.wait();
//    return app.exec();
//}
//
