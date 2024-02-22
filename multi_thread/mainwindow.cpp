#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <pcie_api/ald8xxapi.h>

#include "mythread.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int ret = 0;
    ret = device_pcie_init();

    uint32_t value = 0;//连续采集
    control_collect_model(&value, 0);
    uint32_t value1 = 0;//内部时钟
    control_collect_clock_source(&value1, 0);
    uint32_t value2 = 2;//设置外部触发源
    control_trigger_source(&value2, 0);

    ui->setupUi(this);

    qDebug() << "主线程对象地址:  " << QThread::currentThread();
            // 创建子线程
            MyThread* subThread = new MyThread;

    connect(subThread, &MyThread::curNumber, this, [=](int num)
            {
                ui->label->setNum(num);
            });

    connect(ui->startBtn, &QPushButton::clicked, this, [=]()
            {
                // 启动子线程
                subThread->start();
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}
