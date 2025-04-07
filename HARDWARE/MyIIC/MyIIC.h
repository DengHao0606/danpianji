#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f4xx.h"                  // Device header
#include "System.h"

#define    IIC_IO_SDA      GPIO_Pin_12  //SDA的IO口
#define    IIC_IO_SCL      GPIO_Pin_1  //SCL的IO口
#define    GPIOX           GPIOB       //GPIOx选择
#define    CLOCK		   RCC_APB2Periph_GPIOB //时钟信号
 
#define    IIC_SCL         PBout(6) //SCL
#define    IIC_SDA         PBout(7) //输出SDA
#define    READ_SDA        PBin(7)  //输入SDA

void MyI2C_W_SCL(uint8_t BitValue);
void MyI2C_W_SDA(uint8_t BitValue);
void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void IIC_init(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_ack(void);
void IIC_noack(void);
uint8_t IIC_wait_ack(void);
void IIC_send_byte(uint8_t txd);
uint8_t IIC_read_byte(uint8_t ack);

#endif
