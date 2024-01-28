#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QDebug"
#include <QElapsedTimer>

//#include <pcie_api/ald8xxapi_function.h>
#include <pcie_api/ald8xxapi.h>
#include <data_config.h>
//#include <mythread.h>
//#include <dynamicdraw.h>
//软件版本
#define SOFTWARE_VERSION 1.1
//调试开关
//#define DEBUG 1
//使用线程
//#define USE_THREAD 1
//采集通道个数
#define CHANNLE_NUM 4
//定时器
#define TIME_OUT 1

//static HINSTANCE handle_lib = NULL;


static uint8_t initial_flag_ok = 0;//设置初始化标志位 用于指示初始化状态
static uint8_t setting_flag_ok = 0;//设置 设置完成标志 用于指示采集可以开始
static uint8_t collecting_flag_ok = 0;//用于指示采集状态的标志位
static uint8_t tid_receive_quit;

struct write_data_list write_data;
struct read_data_list read_data;

char receive_data_buffer[16384*4];
char receive_ch1_buffer[32768];
char receive_ch2_buffer[32768];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("PCIE CONTROLER(V%1)").arg(QString::number(SOFTWARE_VERSION)));

    //初始化时获取硬件版本号
    get_api_version(read_data.version_id,sizeof(read_data.version_id));
   if(get_api_version(read_data.version_id, sizeof(read_data.version_id))>0)
   {
       QString version_ID = QString("采集卡版本为：%1").arg(read_data.version_id);
       ui->message_plainTextEdit->appendPlainText(version_ID);
   }
   else
   {
       ui->message_plainTextEdit->appendPlainText("获取采集卡版本失败！");
   }
   //获取ADC最大采样率,显示在输入栏中
   read_data.max_sample_rate = get_sampling_rate();
   ui->sample_rate_text->setText(QString::number(read_data.max_sample_rate));

   //设置combox分组
   QStringList sample_mode_list;
   sample_mode_list<<"触发采集"<<"连续采集";
   ui->sample_mode_comboBox->addItems(sample_mode_list);

   QStringList clock_mode_list;
   clock_mode_list<<"内部时钟"<<"外部时钟";
   ui->clock_mode_comboBox->addItems(clock_mode_list);

   QStringList trigger_mode_list;
   trigger_mode_list<<"软件触发"<<"外部触发"<<"通道触发";
   ui->trigger_mode_comboBox->addItems(trigger_mode_list);

   //在未进行初始化的时候，参数设置框都为灰色的
   ui->start_collect_Button->setEnabled(0);
   ui->sample_set_frame->setEnabled(0);
   ui->triger_set_frame->setEnabled(0);

   //初始化设备状态显示
   ui->device_status_text->setStyleSheet(" background-color: rgb(255, 0, 0);");
   ui->device_status_text->setText("设备断开");
   ui->collect_status_text->setStyleSheet(" background-color: rgb(255, 0, 0);");
   ui->collect_status_text->setText("停止采集");
   ui->triger_length_text->setText(QString::number(1024));
   //ui->ch1_checkBox->setChecked(true);
   //ui->ch1_checkBox->setChecked(true);
   //绘图区初始化
   RealtimeDataShow(ui->customPlot);

   //定时器初始化
   dataTimer = new QTimer(this);
   connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
   dataTimer->start(TIME_OUT);

#ifdef USE_THREAD
   //创建线程对象
   GetSampleData* GetDataThread = new GetSampleData;
   connect(this,&MainWindow::starting,GetDataThread,&GetSampleData::recdata);//把主线程的参数传递到子线程当中
   connect(ui->start_collect_Button,&QPushButton::clicked,this,[=]()
   {
        emit starting(tid_receive_quit);
        GetDataThread->start();
    });

   connect(GetDataThread,&GetSampleData::sendArry,this,[=](QVector<int> data_list_ch1,QVector<int> data_list_ch2)
   {
       for(int i=0;i<data_list_ch1.size();i++)
       {
           ui->customPlot->graph(0)->addData(i,data_list_ch1.at(i));
       }

       for(int i=0;i<data_list_ch2.size();i++)
       {
            ui->customPlot->graph(1)->addData(i,data_list_ch1.at(i));
       }
   });
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
}

//绘图区初始化
void MainWindow::RealtimeDataShow(QCustomPlot *customPlot)
{
    /*
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
*/

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);    //允许可缩放，移动
    //CustomDraw_ch1 = new DynamicDraw(ui->customPlot,this);
    //CustomDraw_ch2 = new DynamicDraw(ui->customPlot,this);
    //初始化分配缓存区
    //CustomDraw_ch1->allotBuffer(QString(ui->triger_length_text->text()).toUInt(),0);
   // CustomDraw_ch2->allotBuffer(QString(ui->triger_length_text->text()).toUInt(),0);
}
//绘图定时器响应函数，在定时器中进行线程的开关和回调函数的数据获取
void MainWindow::realtimeDataSlot()
{
#ifdef DEBUG
        qDebug("开始采集标志:%d",collecting_flag_ok);
#endif

    if(collecting_flag_ok == 1)
    {
       //开启线程
       tid_receive_quit = 1;
    }
    else
    {
       //关闭线程
       tid_receive_quit = 0;
    }

    //画图
    //CustomDraw_ch1->startDraw(1,1);
    //CustomDraw_ch2->startDraw(1,1);

    ui->customPlot->replot();
}
//该按键主要控制 触发设备初始化 以及逆初始化
void MainWindow::on_open_device_Button_toggled(bool checked)
{
    static uint8_t button_flag;
    button_flag = checked;
#ifdef DEBUG
        qDebug("打开设备按键flag:%d，checked：%d",button_flag,checked);
#endif

    if(checked)
    {
        ui->open_device_Button->setText("关闭设备");

        read_data.device_init_return = device_pcie_init();
        if(read_data.device_init_return == 0)
        {
            ui->message_plainTextEdit->appendPlainText("采集卡已打开");
            ui->device_status_text->setStyleSheet(" background-color: rgb(0, 255, 0);");
            ui->device_status_text->setText("设备连接");
            initial_flag_ok = 1; //初始化成功置1
        }
        else
        {
            QString initial_Return = QString("采集卡打开失败！").arg(QString::number(read_data.device_init_return));
            ui->message_plainTextEdit->appendPlainText(initial_Return);
            ui->device_status_text->setStyleSheet(" background-color: rgb(255, 0, 0);");
            ui->device_status_text->setText("设备断开");
            initial_flag_ok = 0; //初始化成功置0
        }
    }else
    {
        ui->open_device_Button->setText("打开设备");
        if(device_pcie_deinit() == 0)
        {
            ui->message_plainTextEdit->appendPlainText("采集卡已关闭");
            ui->device_status_text->setStyleSheet(" background-color: rgb(255, 0, 0);");
            ui->device_status_text->setText("设备断开");
            initial_flag_ok = 0; //初始化成功置0
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText("采集卡关闭失败！");
            ui->device_status_text->setStyleSheet(" background-color: rgb(0, 255, 0);");
            ui->device_status_text->setText("设备连接");
            initial_flag_ok = 1;
        }
    }
    //当初始化成功控件才有效,不在初始化范围内操作函数会崩溃
    ui->start_collect_Button->setEnabled(initial_flag_ok);
    ui->sample_set_frame->setEnabled(initial_flag_ok);
    ui->triger_set_frame->setEnabled(initial_flag_ok);
}


void MainWindow::on_write_all_commend_Button_clicked()
{
#ifdef DEBUG
        qDebug("打开一键设置按键");
#endif
    //采样率设置
    QString sample_fre_text = ui->sample_rate_text->text();
    write_data.control_sample_fre[0] = sample_fre_text.toUInt();

    //采集模式设置
    QString sample_mode_text = ui->sample_mode_comboBox->currentText();
    if(sample_mode_text == "连续采集")
    {
        write_data.control_collect_mode[0] = 1;
        ui->triger_set_frame->setEnabled(0);//连续采集中无法使用触发采集

    }
    else if(sample_mode_text == "触发采集")
    {
        write_data.control_collect_mode[0] = 0;
        ui->triger_set_frame->setEnabled(1);//使能触发采集
    }
    //时钟选择设置
    QString clock_mode_text = ui->clock_mode_comboBox->currentText();
    if(clock_mode_text == "外部时钟")
    {
        write_data.control_clock_source[0] = 1;
    }
    else if(clock_mode_text == "内部时钟")
    {
        write_data.control_clock_source[0] = 0;
    }
    //触发长度设置
    QString triger_length_text = ui->triger_length_text->text();
    write_data.trigger_length[0] = triger_length_text.toUInt()*CHANNLE_NUM;
    //触发源选择设置
    QString trigger_mode_text = ui->trigger_mode_comboBox->currentText();
    if(trigger_mode_text == "软件触发")
    {
        write_data.trigger_source[0] = 0;
        ui->triger_set_frame->setEnabled(1);
    }
    else if(trigger_mode_text == "通道触发")
    {
        write_data.trigger_source[0] = 1;
        ui->triger_set_frame->setEnabled(0);
    }
    else if(trigger_mode_text == "外部触发")
    {
        write_data.trigger_source[0] = 2;
        ui->triger_set_frame->setEnabled(0);
    }

#ifdef DEBUG
    qDebug("采样率为%d",write_data.control_sample_fre[0]);
    qDebug("采集模式为%d",write_data.control_collect_mode[0]);
    qDebug("时钟模式为%d",write_data.control_clock_source[0]);
    qDebug("触发长度为%d",write_data.trigger_length[0]);
    qDebug("触发源为%d",write_data.trigger_source[0]);
#endif

    //所有参数设置
    if(initial_flag_ok)
    {
        read_data.sample_fre_return = control_sampling_frequency(write_data.control_sample_fre,_WRITE_);
        read_data.collect_mode_return = control_sampling_frequency(write_data.control_collect_mode,_WRITE_);
        read_data.clock_source_return = control_sampling_frequency(write_data.control_clock_source,_WRITE_);
        read_data.trigger_length_return = control_trigger_length(write_data.trigger_length,_WRITE_);
        read_data.trigger_source_return = control_trigger_source(write_data.trigger_source,_WRITE_);


        if((read_data.sample_fre_return>=0)&&
            (read_data.collect_mode_return>=0)&&
            (read_data.clock_source_return>=0)&&
            (read_data.trigger_length_return>=0)&&
            (read_data.trigger_source_return>=0))
        {
            ui->message_plainTextEdit->appendPlainText("一键设置成功");
            setting_flag_ok = 1;//完成参数设置 置1
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText("一键设置失败");
            setting_flag_ok = 0;//完成参数设置 置0
        }
    }
    else
    {
        ui->message_plainTextEdit->appendPlainText("一键设置失败");
        setting_flag_ok = 0;//完成参数设置 置0
    }
}

void MainWindow::on_write_rate_clock_Button_clicked()
{
#ifdef DEBUG
        qDebug("打开采样率设置按键");
#endif
    //采样率设置
    QString sample_fre_text = ui->sample_rate_text->text();
    write_data.control_sample_fre[0] = sample_fre_text.toUInt();

#ifdef DEBUG
    qDebug("采样率为%d",write_data.control_sample_fre[0]);
#endif

    if(initial_flag_ok)
    {
        read_data.sample_fre_return = control_sampling_frequency(write_data.control_sample_fre,_WRITE_);
        if(read_data.sample_fre_return>=0)
        {
            ui->message_plainTextEdit->appendPlainText("采样率设置成功");
            setting_flag_ok = 1;//完成参数设置 置1
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText("采样率设置失败");
            setting_flag_ok = 0;//完成参数设置 置0
        }
    }
    else
    {
        ui->message_plainTextEdit->appendPlainText("采样率设置失败");
        setting_flag_ok = 0;//完成参数设置 置0
    }
    //采集模式设置
    QString sample_mode_text = ui->sample_mode_comboBox->currentText();
    if(sample_mode_text == "连续采集")
    {
        write_data.control_collect_mode[0] = 1;
        ui->triger_set_frame->setEnabled(0);//连续采集中无法使用触发采集
    }
    else if(sample_mode_text == "触发采集")
    {
        write_data.control_collect_mode[0] = 0;
        ui->triger_set_frame->setEnabled(1);//连续采集中无法使用触发采集
    }
#ifdef DEBUG
    qDebug("采集模式为%d",write_data.control_collect_mode[0]);
#endif
    if(initial_flag_ok)
    {
        read_data.collect_mode_return = control_sampling_frequency(write_data.control_collect_mode,_WRITE_);
        if(read_data.collect_mode_return>=0)
        {
            ui->message_plainTextEdit->appendPlainText(sample_mode_text+"设置成功");
            setting_flag_ok = 1;//完成参数设置 置1
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText(sample_mode_text+"设置失败");
            setting_flag_ok = 0;//完成参数设置 置0
        }
    }
    else
    {
        ui->message_plainTextEdit->appendPlainText(sample_mode_text+"设置失败");
        setting_flag_ok = 0;//完成参数设置 置0
    }
    //时钟选择设置
    QString clock_mode_text = ui->clock_mode_comboBox->currentText();
    if(clock_mode_text == "外部时钟")
    {
        write_data.control_clock_source[0] = 1;
    }
    else if(clock_mode_text == "内部时钟")
    {
        write_data.control_clock_source[0] = 0;
    }
#ifdef DEBUG
    qDebug("时钟模式为%d",write_data.control_clock_source[0]);
#endif
    if(initial_flag_ok)
    {
        read_data.clock_source_return = control_sampling_frequency(write_data.control_clock_source,_WRITE_);
        if(read_data.clock_source_return>=0)
        {
            ui->message_plainTextEdit->appendPlainText(clock_mode_text+"设置成功");
            setting_flag_ok = 1;//完成参数设置 置1
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText(clock_mode_text+"设置失败");
            setting_flag_ok = 0;//完成参数设置 置0
        }
    }
    else
    {
        ui->message_plainTextEdit->appendPlainText(clock_mode_text+"设置失败");
        setting_flag_ok = 0;//完成参数设置 置0
    }

}


void MainWindow::on_write_triger_length_Button_clicked()
{
#ifdef DEBUG
        qDebug("打开触发采集设置按键");
#endif
    //触发长度设置
    QString triger_length_text = ui->triger_length_text->text();
    write_data.trigger_length[0] = triger_length_text.toUInt()*CHANNLE_NUM;

#ifdef DEBUG
    qDebug("触发长度为%d",write_data.trigger_length[0]);
#endif
    if(initial_flag_ok)
    {
        read_data.trigger_length_return = control_trigger_length(write_data.trigger_length,_WRITE_);
        if(read_data.trigger_length_return>=0)
        {
            ui->message_plainTextEdit->appendPlainText("触发长度设置成功");
            setting_flag_ok = 1;//完成参数设置 置1
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText("触发长度设置失败");
            setting_flag_ok = 0;//完成参数设置 置0
        }
    }else
    {
        ui->message_plainTextEdit->appendPlainText("触发长度设置失败");
        setting_flag_ok = 0;//完成参数设置 置0
    }


    //触发源选择设置
    QString trigger_mode_text = ui->trigger_mode_comboBox->currentText();
    if(trigger_mode_text == "软件触发")
    {
        write_data.trigger_source[0] = 0;
        ui->triger_set_frame->setEnabled(1);
    }
    else if(trigger_mode_text == "通道触发")
    {
        write_data.trigger_source[0] = 1;
        ui->triger_set_frame->setEnabled(0);
    }
    else if(trigger_mode_text == "外部触发")
    {
        write_data.trigger_source[0] = 2;
        ui->triger_set_frame->setEnabled(0);
    }
#ifdef DEBUG
    qDebug("触发源为%d",write_data.trigger_source[0]);
#endif
    if(initial_flag_ok)
    {
        read_data.trigger_source_return = control_trigger_source(write_data.trigger_source,_WRITE_);
        if(read_data.trigger_source_return>=0)
        {
            ui->message_plainTextEdit->appendPlainText(trigger_mode_text+"设置成功");
            setting_flag_ok = 1;//完成参数设置 置1
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText(trigger_mode_text+"设置失败");
            setting_flag_ok = 0;//完成参数设置 置0
        }
    }
    else
    {
        ui->message_plainTextEdit->appendPlainText(trigger_mode_text+"设置失败");
        setting_flag_ok = 0;//完成参数设置 置0
    }
}

static uint8_t soft_trigger_count = 0;

void MainWindow::on_soft_triger_Button_clicked(bool checked)
{
    static uint8_t button_flag;
    button_flag = checked;
#ifdef DEBUG
    qDebug("打开软件触发采集按键%d,%d,%d,%d",collecting_flag_ok,button_flag,setting_flag_ok,write_data.trigger_source[0]);
#endif
    if(collecting_flag_ok&&(button_flag == 0)&&setting_flag_ok&&(write_data.trigger_source[0] == 0))
    {
        write_data.software_trigger[0] = 1;
        read_data.software_trigger_return = control_software_trigger(write_data.software_trigger,_WRITE_);
        if(read_data.software_trigger_return>=0)
        {
            QElapsedTimer time; //计算程序运行时间
            int milsec;//运行时间计时
            soft_trigger_count = soft_trigger_count + 1;
            read_data.single_start_sample_return = single_start_sample(receive_data_buffer,QString(ui->triger_length_text->text()).toInt()*CHANNLE_NUM, 10);
            //清理数据
            ui->customPlot->graph(0)->data().data()->clear();
            ui->customPlot->graph(1)->data().data()->clear();
            //分离数据 画图
            int mun = 0;
            for(int i=0;i<read_data.single_start_sample_return;i+=4)
            {
                    //receive_ch1_buffer[i%2]=receive_data_buffer[i];
                    ui->customPlot->graph(1)->addData(mun,receive_data_buffer[i+1]);
                   //CustomDraw_ch1->addData(receive_data_buffer[i]);

//                    //receive_ch1_buffer[i%2]=receive_data_buffer[i];
                    ui->customPlot->graph(0)->addData(mun,receive_data_buffer[i+3]);
//                   //CustomDraw_ch2->addData(receive_data_buffer[i]);
                    mun++;
            }
            milsec = time.elapsed();
            qDebug()<< "数据分离时间"<<milsec<<"milsec";
        ui->message_plainTextEdit->appendPlainText(QString("软件成功触发%1次").arg(QString::number(soft_trigger_count)));
        }
        else
        {
            ui->message_plainTextEdit->appendPlainText("软件触发失败");
        }
    }
    else if(button_flag == 1)
    {
        write_data.software_trigger[0] = 0;
        read_data.software_trigger_return = control_software_trigger(write_data.software_trigger,_WRITE_);
        ui->message_plainTextEdit->appendPlainText("软件触发失败");
    }


#ifdef DEBUG
    qDebug("软件触发采集按键值为%d",write_data.software_trigger[0]);
#endif
}

void MainWindow::on_read_all_commend_Button_clicked()
{

}


void MainWindow::on_read_triger_length_Button_clicked()
{

}


void MainWindow::on_read_rate_clock_Button_clicked()
{

}


//开始采集按键
void MainWindow::on_start_collect_Button_toggled(bool checked)
{
    static uint8_t button_flag;
    button_flag = checked;
#ifdef DEBUG
        qDebug("开始采集按键flag:%d，checked：%d",button_flag,checked);
#endif
    if(button_flag&setting_flag_ok&initial_flag_ok) //初始化成功、参数设置以及采集按键按下 才能进行数据采集
    {
        ui->start_collect_Button->setText("停止采集");

        write_data.control_collect_en[0] = 1;//采集使能
        read_data.collect_enable_return = control_collect_enable(write_data.control_collect_en,_WRITE_);
        if(read_data.collect_enable_return >= 0)
        {
            //dataTimer->start(TIME_OUT); //打开定时器进行数据获取
            collecting_flag_ok = 1;

            ui->message_plainTextEdit->appendPlainText("开始进行数据采集");
            ui->collect_status_text->setStyleSheet(" background-color: rgb(0, 255, 0);");
            ui->collect_status_text->setText("开始采集");
        }
    }else
    {
        ui->start_collect_Button->setText("开始采集");

        write_data.control_collect_en[0] = 0;//采集停止
        read_data.collect_enable_return = control_collect_enable(write_data.control_collect_en,_WRITE_);
        if(read_data.collect_enable_return >= 0)
        {
           // dataTimer->stop();  //关闭定时器停止数据采集

            collecting_flag_ok = 0;
            ui->message_plainTextEdit->appendPlainText("停止进行数据采集");
            ui->collect_status_text->setStyleSheet(" background-color: rgb(255, 0, 0);");
            ui->collect_status_text->setText("停止采集");

        }
    }
}

void MainWindow::on_graphics_clean_Button_clicked()
{
    //ui->customPlot->clearGraphs();
    ui->customPlot->graph(0)->data().data()->clear();
    ui->customPlot->graph(1)->data().data()->clear();
}


void MainWindow::on_ch1_checkBox_toggled(bool checked)
{
    static uint8_t button_flag;
    button_flag = checked;
    if(button_flag)
    {
        ui->customPlot->graph(1)->setVisible(true);
    }
    else
    {
        ui->customPlot->graph(1)->setVisible(false);
    }
}


void MainWindow::on_ch2_checkBox_toggled(bool checked)
{
    static uint8_t button_flag;
    button_flag = checked;
    if(button_flag)
    {
        ui->customPlot->graph(0)->setVisible(true);
    }
    else
    {
        ui->customPlot->graph(0)->setVisible(false);
    }
}


void MainWindow::on_message_clean_Button_clicked()
{
    ui->message_plainTextEdit->clear();
}

