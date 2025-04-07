#include "MyIIC.h"

/**
  * 函    数：I2C写SCL引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SCL的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SCL为低电平，当BitValue为1时，需要置SCL为高电平
  */
void MyI2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6 ,(GPIO_PinState)BitValue);		//根据BitValue，设置SCL引脚的电平
	HAL_Delay_us(10);												//延时10us，防止时序频率超过要求
}

/**
  * 函    数：I2C写SDA引脚电平
  * 参    数：BitValue 协议层传入的当前需要写入SDA的电平，范围0~1
  * 返 回 值：无
  * 注意事项：此函数需要用户实现内容，当BitValue为0时，需要置SDA为低电平，当BitValue为1时，需要置SDA为高电平
  */
void MyI2C_W_SDA(uint8_t BitValue)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (GPIO_PinState)BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	HAL_Delay_us(10);												//延时10us，防止时序频率超过要求
}

void I2C_SDA_OUT(void)//SDA输出方向配置
{
//  GPIO_InitTypeDef GPIO_InitStructure;	
//	//RCC_APB2PeriphClockCmd(CLOCK,ENABLE);
//	GPIO_InitStructure.GPIO_Pin=IIC_IO_SDA;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//SDA推挽输出
//	GPIO_Init(GPIOX,&GPIO_InitStructure); 						
}

void I2C_SDA_IN(void)//SDA输入方向配置
{
//	GPIO_InitTypeDef GPIO_InitStructure;	
//	//RCC_APB2PeriphClockCmd(CLOCK,ENABLE);
//	GPIO_InitStructure.GPIO_Pin=IIC_IO_SDA;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//SCL上拉输入
//	GPIO_Init(GPIOX,&GPIO_InitStructure);
}

//模拟IIC
void IIC_init()
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(CLOCK, ENABLE);	 //使能PD端口
//	GPIO_InitStructure.GPIO_Pin = IIC_IO_SDA |IIC_IO_SCL;	//PD6推挽输出,SCL
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO速度50MHz
//	GPIO_Init(GPIOX, &GPIO_InitStructure);					 //选择GPIO
//	GPIO_SetBits(GPIOX,IIC_IO_SDA|IIC_IO_SCL); 
}

void IIC_start()	//起始信号
{
	I2C_SDA_OUT();
	MyI2C_W_SDA(1);	  	  
	MyI2C_W_SCL(1);
	HAL_Delay_us(5);
	MyI2C_W_SDA(0);
	HAL_Delay_us(5);
	MyI2C_W_SCL(0);
}

void IIC_stop()		//终止信号
{
	I2C_SDA_OUT();
	MyI2C_W_SCL(0);
	MyI2C_W_SDA(0);
	HAL_Delay_us(5);
	MyI2C_W_SCL(1); 
	MyI2C_W_SDA(1);
	HAL_Delay_us(5);
}

//主机产生应答信号
void IIC_ack()
{
	MyI2C_W_SCL(0);
	I2C_SDA_OUT();
  MyI2C_W_SDA(0);	
	HAL_Delay_us(2);
  MyI2C_W_SCL(1);
	HAL_Delay_us(2);
  MyI2C_W_SCL(0);	
}

//主机不产生应答信号
void IIC_noack()
{
	MyI2C_W_SCL(0);
	I2C_SDA_OUT();
  MyI2C_W_SDA(1);
	HAL_Delay_us(2);
  MyI2C_W_SCL(1);
	HAL_Delay_us(2);
  MyI2C_W_SCL(0);
}

//等待从机接收信号
//返回值：0失败
//		  1成功
uint8_t IIC_wait_ack()
{
	uint8_t tempTime=0;
	I2C_SDA_IN();
	MyI2C_W_SDA(1);
	HAL_Delay_us(1);
	MyI2C_W_SCL(1);
	HAL_Delay_us(1);

	while(READ_SDA)
	{
		tempTime++;
		if(tempTime>250)
		{
			IIC_stop();
			return 1;
		}	 
	}

	MyI2C_W_SCL(0);
	return 0;
}

void IIC_send_byte(uint8_t txd)
{
	uint8_t i=0;
	I2C_SDA_OUT();
	MyI2C_W_SCL(0);;//拉低时钟开始数据传输
	for(i=0;i<8;i++)
	{
		IIC_SDA=(txd&0x80)>>7;//读取字节
		txd<<=1;
		HAL_Delay_us(2);
		MyI2C_W_SCL(1);
		HAL_Delay_us(2); //发送数据
		MyI2C_W_SCL(0);
		HAL_Delay_us(2);
	}
}

//读取一个字节
uint8_t IIC_read_byte(uint8_t ack)
{
	uint8_t i=0,receive=0;
	I2C_SDA_IN();
  for(i=0;i<8;i++)
  {
   	MyI2C_W_SCL(0);
		HAL_Delay_us(2);
		MyI2C_W_SCL(1);
		receive<<=1;//左移
		if(READ_SDA)
		   receive++;//连续读取八位
		HAL_Delay_us(1);	
  }

  if(!ack)
	  IIC_noack();
	else
		IIC_ack();

	return receive;//返回读取到的字节
}
