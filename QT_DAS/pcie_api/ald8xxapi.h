#ifndef ALD8XXAPI_H
#define ALD8XXAPI_H
#ifdef __cplusplus
 extern "C" {
#endif

#if _WIN32
#if defined(ALD8XXAPI_EXPORTS)
#define ALD8XXAPI_EXPORT  __declspec (dllexport)
#else
#define ALD8XXAPI_EXPORT  __declspec (dllimport)
#endif
#else
#if defined(ALD8XXAPI_EXPORTS)
#define ALD8XXAPI_EXPORT   __attribute__((visibility("default")))
#else
#define ALD8XXAPI_EXPORT   __attribute__((visibility("default")))
#endif
#endif


 #define USER_REG_IO_DIR  1056   //IO方向
 #define USER_REG_LVDS_START   1060   //LVDS测试启动
 #define USER_REG_LVDS_RESULT   1064   //LVDS测试结果
 #define USER_REG_UDP_START   1068   //UDP启动

 /**
* @brief: get_api_version API接口版本读取,
* @param:char *buffer API版本内容数据区
* @param:int length 数据区的长度,块数需要大于6
* @retval: int >0 表示成功，<=0 表示失败
 */
 ALD8XXAPI_EXPORT int get_api_version( char *buffer,int length);

 /**
  * 打开设备
  *
  * @return 0：打开成功 其他:打开失败
  */
 ALD8XXAPI_EXPORT int  device_pcie_init();
 /**
  * 关闭设备
  *
  *@return 0：关闭成功
  */
 ALD8XXAPI_EXPORT int  device_pcie_deinit();
 ///////////////////////////////////////////////////////////////////////////////////////////
 //ADC模块
 ///////////////////////////////////////////////////////////////////////////////////////////
 typedef void (*pcie_recv_callback)( const char* data, int length,const void *pContent);
 /**
  * ADC最大采样率
  *
  * @return 0：
  */

ALD8XXAPI_EXPORT unsigned int  get_sampling_rate();

 /**
  * (读取)设置 采集使能 buffer=0:停止采集,buffer=1:启动采集 启动或者停止设备的数据上传
  *
  * @param buffer 采集使能
  * @param is_read is_read=true时为读取,is_read=false时为设置
  *@return >0 (读取)设置 成功
  */
ALD8XXAPI_EXPORT int  control_collect_enable(unsigned int  *buffer,int is_read);
/**
 * (读取)设置 ADC采样率
 *
 * @param buffer 采样率
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int  control_sampling_frequency(unsigned int  *buffer,const int is_read);
/**
 * (读取)设置 采集模式 buffer=0:触发采集,buffer=1:连续采集
 *
 * @param buffer 采集模式
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int  control_collect_model(unsigned int  *buffer,const int is_read);
/**
 * (读取)设置 时钟源 buffer=0:内部时钟,buffer=1:外部时钟
 *
 * @param buffer 时钟源
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int control_collect_clock_source(unsigned int  *buffer,const int is_read);
/**
 * (读取)设置 触发源 buffer=0:软件触发,buffer=1:通道触发,buffer=2:外触发
 *
 * @param buffer 触发源
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int  control_trigger_source(unsigned int  *buffer,const int is_read);
/**
 * (读取)设置 触发边沿 buffer=0:上升沿,buffer=1:下降沿,buffer=2:双边沿
 *
 * @param buffer 触发边沿
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int  control_trigger_clock_edge(unsigned int  *buffer,const int is_read);
/**
 * (读取)设置 软件触发 buffer=0:软件不使能,buffer=1:软件使能
 *
 * @param buffer 软件触发
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int  control_software_trigger(unsigned int  *buffer,const int is_read);

/**
 * (读取)设置 触发长度(pts)
 *
 * @param buffer 触发长度
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int  control_trigger_length(unsigned int  *buffer,const int is_read);

/**
 * (读取)设置 高低翻转 buffer=0:禁用,buffer=1:启用
 *
 * @param buffer 高低翻转
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int control_trigger_output_polarity(unsigned int  *buffer,const int is_read);
/**
 * (读取)设置 通道触发输出 buffer=0:禁用,buffer=4:跟随输出:
 *
 * @param buffer 通道触发输出
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int  control_trigger_output_channel(unsigned int  *buffer,const int is_read);
/**
 * (读取)设置触发输入通道 buffer=0:禁用,buffer=1:模拟触发输入，2,数字触发输入
 *
 * @param buffer 触发输入通道
 * @param is_read is_read=true时为读取,is_read=false时为设置
 *@return >0 (读取)设置 成功
 */
ALD8XXAPI_EXPORT int control_trigger_input_channel(unsigned int  *buffer, const int  is_read);
/**=============回调版本==============**/
/**
 * 绑定数据回调函数并触发开始采集 开启线程接收解析数据
 *
 * @param _pcie_recv_callback  回调函数的地址
 *@return >0 绑定成功
 */
ALD8XXAPI_EXPORT int  start_sample( pcie_recv_callback _pcie_recv_callback,const void *pContent);
/**
 * 停止采集 停止设备上传数据和停止线程接收解析数据
 *
 *@return >0 停止采集成功
 */
ALD8XXAPI_EXPORT int   stop_sample();

/**=============回调版本==============**/
/**
 * single_start_sample 单次采集，采集指定数据长度的数据
 *
 * @param  char* buffer 数据缓存区 short
 * @param int length 数据缓存大小,    //连续采集时，长度不能低于1019*4*2，
 *                                    触发采集时，长度不能需要大于等于触发长度+预触发长度
 * @param  timeout 读取数据等待超时时间
 *@return >0返回采集读取的字节数
 */
ALD8XXAPI_EXPORT int   single_start_sample( char*buffer, int length, int timeout);


/**
 * @brief user_write 写入用户空间寄存器值
 * @param long address 用户空间地址偏移
 * @param void *buffer 用于写入寄存器值的指针
 *  @param int len 用于写入寄存器值的长度
 * @return int 写入成功，返回4；写入失败，返回负值。-1 写入操作失败，-2 写入数据长度错误，-3 设置偏移地址address失败
 */
ALD8XXAPI_EXPORT int pcie_user_write(long address,void *buffer,int len);
/**
 * @brief user_read 读取用户空间寄存器值
 * @param long address 用户空间地址偏移
 * @param int *val 用于存储寄存器值的指针
 * @return int 读取成功，返回4；读取失败，返回负值。-1 读取操作失败，-2 读取数据长度错误，-3 设置偏移地址address失败
 */
ALD8XXAPI_EXPORT int pcie_user_read(long address, unsigned int *val,int len);



#ifdef __cplusplus
}
#endif
#endif /** ALD81XAPI_H*/
