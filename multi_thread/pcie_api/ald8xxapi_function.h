#ifndef ALD81XAPI_FUNCTION_H
#define ALD81XAPI_FUNCTION_H
#include "stdint.h"
#ifdef __cplusplus
 extern "C" {
#endif



	 typedef void(*pcie_recv_callback)(const char* data, int length, const void *pContent);


/**
* @brief: get_api_version API接口版本读取,
* @param:char *buffer API版本内容数据区
* @param:int length 数据区的长度,块数需要大于6
* @retval: int >0 表示成功，<=0 表示失败
*/
typedef int(__cdecl * get_api_version)(char *buffer, int length);

 /**
  * 打开设备
  *
  * @return 0：打开成功 其他:打开失败
  */
 typedef int(__cdecl *   device_pcie_init)();
 /**
  * 关闭设备
  *
  *@return 0：关闭成功
  */
 typedef int(__cdecl *   device_pcie_deinit)();



 ///////////////////////////////////////////////////////////////////////////////////////////
 //ADC模块
 ///////////////////////////////////////////////////////////////////////////////////////////

 /**
 * ADC最大采样率
 *
 *@return ：最大采样率
 */
 typedef unsigned int(__cdecl *   get_sampling_rate)();

 /**
  * (读取)设置 采集使能 buffer=0:停止采集,buffer=1:启动采集 启动或者停止设备的数据上传
  *
  * @param buffer 采集使能
  * @param is_read is_read=true时为读取,is_read=false时为设置
  *@return >0 (读取)设置 成功
  */
typedef int(__cdecl *   control_collect_enable)(uint32_t *buffer,int is_read);
/**
 * (读取)设置 采样率设置 buffer=0:10Msps,buffer=1:5Msps,buffer=1:1Msps,buffer=1:500Ksps,buffer=1:100Ksps,buffer=1:50Ksps
 *                    buffer=0:10Ksps,buffer=0:1Ksps
 *
 * @param buffer 采样率
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *   control_sampling_frequency)(uint32_t *buffer,const int is_read);
/**
 * (读取)设置 采集模式 buffer=0:触发采集,buffer=1:连续采集
 *
 * @param buffer 采集模式
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *   control_collect_model)(uint32_t *buffer,const int is_read);
/**
 * (读取)设置 时钟源 buffer=0:内部时钟,buffer=1:外部时钟
 *
 * @param buffer 时钟源
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *  control_collect_clock_source)(uint32_t *buffer,const int is_read);
/**
 * (读取)设置 触发源 buffer=0:软件触发,buffer=1:通道触发,buffer=2:外触发
 *
 * @param buffer 触发源
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *   control_trigger_source)(uint32_t *buffer,const int is_read);
/**
 * (读取)设置 触发边沿 buffer=0:上升沿,buffer=1:下降沿,buffer=2:双边沿
 *
 * @param buffer 触发边沿
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *   control_trigger_clock_edge)(uint32_t *buffer,const int is_read);
/**
 * (读取)设置 软件触发 buffer=0:软件不使能,buffer=1:软件使能
 *
 * @param buffer 软件触发
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *   control_software_trigger)(uint32_t *buffer,const int is_read);

/**
 * (读取)设置 触发长度(us)
 *
 * @param buffer 触发长度
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *   control_trigger_length)(uint32_t *buffer,const int is_read);




/**
 * (读取)设置 通道触发输出 buffer=0:禁用,buffer=4:跟随输出:
 *
 * @param buffer 通道触发输出
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
typedef int(__cdecl *   control_trigger_output_channel)(uint32_t *buffer,const int is_read);





/**=============回调版本==============**/
/**
* 绑定数据回调函数并触发开始采集 开启线程接收解析数据
*
* @param _pcie_recv_callback  回调函数的地址
*@return >0 绑定成功
*/
typedef int(__cdecl * start_sample)(pcie_recv_callback _pcie_recv_callback);
/**
* 停止采集 停止设备上传数据和停止线程接收解析数据
*
*@return >0 停止采集成功
*/
typedef int(__cdecl *  stop_sample)();

/**=============回调版本==============**/



/**
 * single_start_sample 单次采集，采集指定数据长度的数据
 *
 * @param  char* buffer 数据缓存区
 * @param int length 数据缓存大小,    //连续采集时，长度不能低于1019*4*2，
 *                                    触发采集时，长度不能需要大于等于触发长度+预触发长度
 * @param  timeout 读取数据等待超时时间
 *@return >0返回采集读取的字节数
 */
typedef int(__cdecl *    single_start_sample)( char*buffer, int length, int timeout);



/**
* @brief user_write 写入用户空间寄存器值
* @param long address 用户空间地址偏移
* @param void *buffer 用于写入寄存器值的指针
*  @param int len 用于写入寄存器值的长度
* @return int 写入成功，返回4；写入失败，返回负值。-1 写入操作失败，-2 写入数据长度错误，-3 设置偏移地址address失败
*/
typedef int(__cdecl *  pcie_user_write)(long address, void *buffer, int len);
/**
* @brief user_read 读取用户空间寄存器值
* @param long address 用户空间地址偏移
* @param int *val 用于存储寄存器值的指针
* @return int 读取成功，返回4；读取失败，返回负值。-1 读取操作失败，-2 读取数据长度错误，-3 设置偏移地址address失败
*/
typedef int(__cdecl *  pcie_user_read)(long address, unsigned int *val, int len);


get_api_version f_get_api_version;
device_pcie_init	f_device_pcie_init;
device_pcie_deinit	f_device_pcie_deinit;
get_sampling_rate   f_get_sampling_rate;
control_collect_enable	f_control_collect_enable;
control_sampling_frequency	f_control_sampling_frequency;
control_collect_model	f_control_collect_model;
control_collect_clock_source	f_control_collect_clock_source;
control_trigger_source	f_control_trigger_source;
control_trigger_clock_edge	f_control_trigger_clock_edge;
control_software_trigger	f_control_software_trigger;

control_trigger_length	f_control_trigger_length;
control_trigger_output_channel	f_control_trigger_output_channel;


single_start_sample	f_single_start_sample;

start_sample	f_start_sample;
stop_sample	f_stop_sample;
pcie_user_write f_pcie_user_write;
pcie_user_read f_pcie_user_read;



#ifdef __cplusplus
}
#endif
#endif /** ALD81XAPI_H*/
