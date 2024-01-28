#ifndef DATA_CONFIG_H
#define DATA_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

struct write_data_list //qt界面 写入参数
{
    //采集使能设置
    uint32_t control_collect_en[1];//启动采集 1 停止采集 0
    //采样率设置
    uint32_t control_sample_fre[1];
    //采集模式设置
    uint32_t control_collect_mode[1];//触发采集 0 连续采集 1
    //时钟源
    uint32_t control_clock_source[1];//内部时钟 0 外部时钟 1
    //单次采集
    int single_start_sample_length;//数据缓存大小
    //ADC触发输入模块
    uint32_t trigger_source[1];//软件触发 0 通道触发 1 外部触发 2
    //边沿触发设置
    uint32_t trigger_clock_edge[1]; //上升沿触发 0 下降沿触发 1 边沿触发 2
    //软件触发使能
    uint32_t software_trigger[1];//软件不使能 0 软件使能 1
    //触发长度设置
    uint32_t trigger_length[1];//触发点数
    //通道触发输出
    uint32_t trigger_output_channel[1];//触发未输出 0 跟随输出 4
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

struct user_data_list //qt界面 读出参数
{
    uint8_t user_addr;
};

enum RETURN_STATUS{
    _SUCCESS_ = 1,
    _FAILURE_ = 0
};

enum READ_WRITE_STATUS{
    _READ_ = 0,
    _WRITE_ = 1
};

extern struct write_data_list write_data;
extern struct read_data_list read_data;
extern struct user_data_list user_data;


#ifdef __cplusplus
}
#endif
#endif // DATA_CONFIG_H



