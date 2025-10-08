
#ifndef LUAT_SPI_H
#define LUAT_SPI_H
#include "luat_base.h"
/**
 * @defgroup luatos_device_spi SPI接口
 * @{
*/
typedef struct luat_spi
{
    int  id;            /**< spi id        可选  1，0*/
    int  CPHA;          /**< CPHA          可选  1，0*/  
    int  CPOL;          /**< CPOL          可选  1，0*/  
    int  dataw;         /**< 数据宽度        8：8bit */
    int  bit_dict;      /**< 高低位顺序     可选  1：MSB，   0：LSB     */  
    int  master;        /**< 设置主从模式   可选  1：主机，  0：从机     */  
    int  mode;          /**< 设置全\半双工  可选  1：全双工，0：半双工    */  
    int bandrate;       /**< 频率           最小100000， 最大25600000*/  
    int cs;             /**< cs控制引脚     如果和硬件cs脚一致，则启用硬件cs功能，反之需要用户自行初始化成gpio功能来控制*/
} luat_spi_t;

typedef struct luat_spi_device
{
    uint8_t  bus_id;
    luat_spi_t spi_config;
    void* user_data;
} luat_spi_device_t;

typedef struct luat_fatfs_spi
{
	uint8_t type;
	uint8_t spi_id;
	uint8_t spi_cs;
	uint8_t nop;
	uint32_t fast_speed;
    uint8_t transfer_buf[7];
	luat_spi_device_t * spi_device;
}luat_fatfs_spi_t;

typedef int (*luat_spi_irq_callback_t)(int spi_id, void *user_data);

/**
    spiId,--spi id
    cs,
    0,--CPHA
    0,--CPOL
    8,--数据宽度
    20000000,--最大频率20M
    spi.MSB,--高低位顺序    可选，默认高位在前
    spi.master,--主模式     可选，默认主
    spi.full,--全双工       可选，默认全双工
*/
/**
 * @brief 初始化配置SPI各项参数，并打开SPI
 * 
 * @param spi spi结构体
 * @return int 成功返回0
 */
int luat_spi_setup(luat_spi_t* spi);
/**
 * @brief SPI收发数据尝试启动DMA模式
 * 
 * @param spi_id spi id
 * @param tx_channel 发送通道
 * @param rx_channel 接收通道
 * @return int
 */
int luat_spi_config_dma(int spi_id, uint32_t tx_channel, uint32_t rx_channel);
/**
 * @brief 关闭SPI
 * 
 * @param spi_id spi id
 * @return int 成功返回0
 */
int luat_spi_close(int spi_id);
/**
 * @brief 收发SPI数据
 * 
 * @param spi_id spi id
 * @param send_buf 发送数据
 * @param send_length 发送数据长度
 * @param recv_buf 接收数据
 * @param recv_length 接收数据长度
 * @return int 返回接收字节数
 */
int luat_spi_transfer(int spi_id, const char* send_buf, size_t send_length, char* recv_buf, size_t recv_length);
/**
 * @brief 收SPI数据
 * 
 * @param spi_id spi id
 * @param recv_buf 接收数据
 * @param length 数据长度
 * @return int 返回接收字节数
 */
int luat_spi_recv(int spi_id, char* recv_buf, size_t length);
/**
 * @brief 发SPI数据
 * 
 * @param spi_id spi id
 * @param send_buf 发送数据
 * @param length 数据长度
 * @return int 返回发送字节数
 */
int luat_spi_send(int spi_id, const char* send_buf, size_t length);
/**
 * @brief SPI速率修改
 * 
 * @param spi_id spi id
 * @param speed 速率
 * @return int 返回发送字节数
 */
int luat_spi_change_speed(int spi_id, uint32_t speed);
/**
 * @brief SPI收发数据(异步)
 * 
 * @param spi_id spi id
 * @param tx_buff 发送数据
 * @param rx_buff 接收数据
 * @param len 数据长度
 * @param CB 回调函数
 * @param pParam 回调参数
 * @return int 返回发送字节数
 */
int luat_spi_no_block_transfer(int spi_id, uint8_t *tx_buff, uint8_t *rx_buff, size_t len, void *CB, void *pParam);
/**
 * @brief 设置从机SPI接收buf满回调函数，制定好SPI协议，尽量不要触发接收BUF满的中断
 *
 * @param spi_id spi id
 * @param callback 回调函数
 * @param user_data 用户数据
 * @return int 成功返回0，其他-1
 */
int luat_spi_set_slave_callback(int spi_id, luat_spi_irq_callback_t callback, void *user_data);
/**
 * @brief 收发从机SPI数据
 *
 * @param spi_id spi id
 * @param send_buf 发送数据
 * @param recv_buf 接收数据
 * @param recv_length 总缓冲区长度，不能大于8188
 * @return int 成功返回0，其他-1
 */
int luat_spi_slave_transfer(int spi_id, const char* send_buf,  char* recv_buf, size_t total_length);
/**
 * @brief 从机SPI暂停工作，并返回已经接收的数据长度，不允许进入休眠状态
 *
 * @param spi_id spi id
 * @return int 成功返回本次接收长度，其他-1
 */
int luat_spi_slave_transfer_pause_and_read_data(int spi_id);
/**
 * @brief 在中断中收发从机SPI数据
 *
 * @param spi_id spi id
 * @param send_buf 发送数据
 * @param recv_buf 接收数据
 * @param recv_length 总缓冲区长度，不能大于8188
 * @return int 成功返回0，其他-1
 */
int luat_spi_slave_fast_transfer_in_irq(int spi_id, const char* send_buf,  char* recv_buf, size_t total_length);
/**
 * @brief 在中断中从机SPI暂停工作，并返回已经接收的数据长度，不允许进入休眠状态
 *
 * @param spi_id spi id
 * @return int 成功返回本次接收长度，其他-1
 */
int luat_spi_slave_transfer_fast_pause_and_read_data_in_irq(int spi_id);
/**
 * @brief 从机SPI暂停工作，不允许进入休眠状态，在irq中使用
 *
 * @param spi_id spi id
 * @return int 成功返回0，其他-1
 */
int luat_spi_slave_transfer_pause_in_irq(int spi_id);
/**
 * @brief 从机SPI停止工作，并允许进入休眠状态
 *
 * @param spi_id spi id
 * @return int 成功返回0，其他-1
 */
int luat_spi_slave_transfer_stop(int spi_id);
/**
 * @brief SPI模式获取
 * 
 * @param spi_id spi id
 * @return int 模式
 */
int luat_spi_get_mode(int spi_id);
/**
 * @brief SPI模式修改
 * 
 * @param spi_id spi id
 * @param mode 模式
 * @return int 返回发送字节数
 */
int luat_spi_set_mode(int spi_id, uint8_t mode);

/**
 * @brief spi总线初始化
 * 
 * @param spi_dev luat_spi_device_t 结构体
 * @return int 
 */
int luat_spi_bus_setup(luat_spi_device_t* spi_dev);
/**
 * @brief spi设备初始化
 * 
 * @param spi_dev luat_spi_device_t 结构体
 * @return int 
 */
int luat_spi_device_setup(luat_spi_device_t* spi_dev);
/**
 * @brief spi设备配置
 * 
 * @param spi_dev luat_spi_device_t 结构体
 * @return int 
 */
int luat_spi_device_config(luat_spi_device_t* spi_dev);
/**
 * @brief spi设备关闭
 * 
 * @param spi_dev luat_spi_device_t 结构体
 * @return int 
 */
int luat_spi_device_close(luat_spi_device_t* spi_dev);
/**
 * @brief spi设备收发数据，返回接收字节数
 * 
 * @param spi_dev luat_spi_device_t 结构体
 * @param send_buf 发送数据
 * @param send_length 发送数据长度
 * @param recv_buf 接收数据
 * @param recv_length 接收数据长度
 * @return int 
 */
int luat_spi_device_transfer(luat_spi_device_t* spi_dev, const char* send_buf, size_t send_length, char* recv_buf, size_t recv_length);
/**
 * @brief spi设备接收数据，返回接收字节数
 * 
 * @param spi_dev luat_spi_device_t 结构体
 * @param recv_buf 接收数据
 * @param length 数据长度
 * @return int 返回接收字节数
 */
int luat_spi_device_recv(luat_spi_device_t* spi_dev, char* recv_buf, size_t length);
/**
 * @brief spi设备发送数据，返回接收字节数
 * 
 * @param spi_dev luat_spi_device_t 结构体
 * @param send_buf 发送数据
 * @param length 数据长度
 * @return int 返回发送字节数
 */
int luat_spi_device_send(luat_spi_device_t* spi_dev, const char* send_buf, size_t length);

/**
 * @brief 锁定SPI，只有主模式下才能使用，多个设备挂载在同一条总线上需要使用
 *
 * @param spi_id spi id
 * @return int 成功返回0，其他-1
 */
int luat_spi_lock(int spi_id);

/**
 * @brief 释放SPI，只有主模式下才能使用，多个设备挂载在同一条总线上需要使用
 *
 * @param spi_id spi id
 * @return int 成功返回0，其他-1
 */
int luat_spi_unlock(int spi_id);
/**@}*/
#endif
