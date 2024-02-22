#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

protected:
    void run();

signals:
    // 自定义信号, 传递数据
    void curNumber(int num);

public slots:
};

struct read_data_list //qt界面 读出参数
{
    //API接口版本读取
    char version_id[10];
    int version_return;
    //初始化设备接口
    int device_init_return;
    //关闭设备接口
    int device_deinit_return;
    //ADC最大采样率
    uint32_t max_sample_rate;
    //采集使能
    uint32_t control_collect_en[1];//启动采集 1 停止采集 0
    int collect_enable_return;
    //采样率设置
    uint32_t control_sample_fre[1];
    int sample_fre_return;
    //采样模式设置
    uint32_t control_collect_mode[1];//触发采集 0 连续采集 1
    int collect_mode_return;
    //时钟源
    uint32_t control_clock_source[1];//内部时钟 0 外部时钟 1
    int clock_source_return;
    //开始采集函数
    int start_sample_return;
    //停止采集
    int stop_sample_return;
    //单次采集
    int single_start_sample_return;
    //ADC触发输入模块
    uint32_t trigger_source[1];//软件触发 0 通道触发 1 外部触发 2
    int trigger_source_return;
    //边沿触发设置
    uint32_t trigger_clock_edge[1]; //上升沿触发 0 下降沿触发 1 边沿触发 2
    int trigger_clock_edge_return;
    //软件触发使能
    uint32_t software_trigger[1];//软件不使能 0 软件使能 1
    int software_trigger_return;
    //触发长度设置
    uint32_t trigger_length[1];//触发点数
    int trigger_length_return;
    //通道触发输出
    uint32_t trigger_output_channel[1];//触发未输出 0 跟随输出 4
    int trigger_output_channel_return;
};
#endif // MYTHREAD_H
