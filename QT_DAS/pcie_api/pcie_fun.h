//! [1]
/**
 * COPYRIGHT (C) 2022, Chengdu MYSOOW Electric CO.,Ltd
 * http://www.mysoow.com
 *
 * Change Logs:
 * Date          Author       Notes
 * ----          Ljay       the first version
 * 2022-02-18    Daisy      add linux APIs and Windosws user space use Mutex
 * …
 */
//! [1]
#ifndef PCIE_FUN_H
#define PCIE_FUN_H
#ifdef __cplusplus
 extern "C" {
 #endif

#if _WIN32
#if defined(PCIE_FUN_EXPORTS)
#define PCIE_FUN_EXPORT  __declspec (dllexport)
#else
#define PCIE_FUN_EXPORT  __declspec (dllimport)
#endif
#else
#if defined(PCIE_FUN_EXPORTS)
#define PCIE_FUN_EXPORT   __attribute__((visibility("default")))
#else
#define PCIE_FUN_EXPORT   __attribute__((visibility("default")))
#endif
#endif


#if _WIN32

 /**
 * @param int channel_mode DMA通道模式，-1，表示通道1，和通道2一起初始化，
										0，表示通道1，初始化
										1，表示通道2，初始化
  * @brief pcie_init 初始化板卡
  * @return int 0：打开成功，-1：没有找到设备，-2：打开发送数据到PC的上行通道设备失败，-3：打开发送数据到FPGA的下行通道设备失败，-4：打开用户空间设备失败
  */
PCIE_FUN_EXPORT int pcie_init( );
/**
 * @param int channel_mode DMA通道模式，-1，表示通道1，和通道2关闭句柄
										0，表示通道1，关闭
										1，表示通道2，关闭
 * @brief pcie_deinit 关闭板卡
 * @return void
 */
PCIE_FUN_EXPORT void pcie_deinit( );
/**
 * @brief user_write 写入用户空间寄存器值
 * @param long address 用户空间地址偏移
 * @param void *buffer 用于写入寄存器值的指针
 *  @param int len 用于写入寄存器值的长度
 * @return int 写入成功，返回4；写入失败，返回负值。-1 写入操作失败，-2 写入数据长度错误，-3 设置偏移地址address失败
 */
PCIE_FUN_EXPORT int user_write(long address,void *buffer,int len);
/**
 * @brief user_read 读取用户空间寄存器值
 * @param long address 用户空间地址偏移
 * @param int *val 用于存储寄存器值的指针
 * @return int 读取成功，返回4；读取失败，返回负值。-1 读取操作失败，-2 读取数据长度错误，-3 设置偏移地址address失败
 */
PCIE_FUN_EXPORT int user_read(long address, unsigned int *val,int len);



/**
 * @param int channel_index DMA通道选择
										0，表示通道1，
										1，表示通道2，
										其他，无效

 * @brief dma_write_transfer 使用DMA向FPGA写入数据
 * @param void *buffer 写入数据的缓冲区地址
 * @param int size 写入数据的大小
 * @return int 写入成功，返回写入的字节数；写入失败，返回负值。-1 写入操作失败，-2 写入数据长度错误，-3 设置偏移地址address失败
 */
PCIE_FUN_EXPORT int dma_write_transfer(long address,const void *buffer, unsigned int size);
/**
 * @brief dma_read_transfer 使用DMA从FPGA读取数据
  * @param int channel_index DMA通道选择
										0，表示通道1，
										1，表示通道2，
										其他，无效
 * @param void *buffer 读取数据的缓冲区地址
 * @param int size 读取数据的大小
 * @return int 读取成功，返回读取的字节数；读取失败，返回负值。-1 读取操作失败，-2 读取数据长度错误，-3 设置偏移地址address失败
 */
PCIE_FUN_EXPORT int dma_read_transfer(long address,void *buffer, unsigned int size);


#else
 PCIE_FUN_EXPORT int pcie_init();

 PCIE_FUN_EXPORT void pcie_deinit();

 PCIE_FUN_EXPORT int user_write(long address,void *buffer,int len);

 PCIE_FUN_EXPORT int user_read(long address,unsigned int *val,unsigned int len);

 PCIE_FUN_EXPORT int dma_write_transfer(long address,const void *buffer,unsigned int size);

 PCIE_FUN_EXPORT int dma_read_transfer( long address,void *buffer,unsigned int size);


#endif

#ifdef __cplusplus
}
#endif
#endif
