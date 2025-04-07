#include "System.h"
#include <math.h>


#define false 0
#define true 1
#define DEBUG 0

typedef unsigned char  byte;
uint8_t result_summ[128];
uint8_t current;

bool changeFunc(uint8_t new_func) 
{
  uint8_t count = 0;
  writeToAddr(0x0035, &new_func, 1);
  HAL_Delay(50);
  while (true) //延时检测当前模式是否更改成功
		{
    if (currentFunc() != new_func) 
			{
				HAL_Delay(50);
      } 
		else 
			{
				return true;
			}
    ++count;
    if (count > 80) 
			{
				return false;
			}
		}
}

int writeToAddr(uint16_t addr, const uint8_t *buf, uint16_t leng) 
{
	int i;
	IIC_start(); //起始信号
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS<<1)); //发送写指令
	IIC_wait_ack(); //等待应答
	IIC_send_byte(addr & 0x00FFu); //寄存器地址高八位
	IIC_wait_ack(); //等待应答
	IIC_send_byte((addr >> 8) & 0x00FFu); //寄存器地址低八位
	for(i=0;i < leng;i++)
	{
		IIC_wait_ack(); //应答
		IIC_send_byte(*buf++); //数组地址
	//	printf("%x\r\n",*buf--);
	}
	IIC_wait_ack(); //等待应答
	IIC_stop(); //停止信号
	return leng;
}

int readFromAddr(uint16_t addr,  uint8_t *buf, uint16_t leng) 
{
  int len = 0,i;

  for (i = 0; i < leng; i++) 
		{
			*buf++ = readByte(addr++); //读取一个字节
      ++len;
    }
//  printf("%d",len);
//  if (ls > 0) {
//	*buf++ = IIC_read_byte(0); 
//      ++len;
//	IIC_wait_ack(); //Ó¦´ð
//    }
  return len;
}

int currentFunc(void) 
{
  char buf;
  readFromAddr(0x0035, &buf, 1);
  return (int)buf;
}

int readByte(uint16_t address)
{
	uint8_t val;
	IIC_start(); //启动
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS<<1)); //发送写指令
	IIC_wait_ack(); //应答
	IIC_send_byte(address & 0x00FFu); //寄存器地址高八位
	IIC_wait_ack(); //应答
	IIC_send_byte((address >> 8) & 0x00FFu); //寄存器地址低八位
	IIC_wait_ack(); //应答
	IIC_stop(); //停止
	IIC_start(); //重新启动
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS<<1)+1); //发送读指令
	IIC_wait_ack(); //应答
	val = IIC_read_byte(0); //读取一个字节
	IIC_stop(); //停止
	return val;
}

void setLed(bool new_state) 
{
  byte buf[3] = {0x30, 0x00, 0x00};
  byte ns_b = new_state ? 1 : 0;
  writeToAddr(0x0030, &ns_b, 1);
}

//更新结果
bool updateResult(void)
{
  readFromAddr(0x0035, &current, 1);
  switch (current) 
		{
			case APPLICATION_FACEDETECT: 
				{
					readFromAddr(0x0400, result_summ, 48);
					break;
				};
			case APPLICATION_OBJDETECT: 
				{
					readFromAddr(0x0800, result_summ, 48);
					break;
				}
			case APPLICATION_CLASSIFICATION: 
				{
					readFromAddr(0x0C00, result_summ, 128);
					break;
				}
			case APPLICATION_FEATURELEARNING: 
				{
					readFromAddr(0x0E00, result_summ, 64);
					break;
				}
			case APPLICATION_COLORDETECT: 
				{
					readFromAddr(0x1000, result_summ, 48);
					break;
				}
			case APPLICATION_LINEFOLLOW: 
				{
					readFromAddr(0x1400, result_summ, 48);
					break;
				}
			case APPLICATION_APRILTAG: 
				{
					readFromAddr(0x1E00, result_summ, 48);
					break;
				}
			case APPLICATION_QRCODE: 
				{
					readFromAddr(0x1800, result_summ, 48);
					break;
				}
			case APPLICATION_BARCODE: 
				{
					readFromAddr(0x1C00, result_summ, 48);
				}
			default: 
				{
					break;
				}
		}
 }
/*是否识别到人脸*/
bool anyFaceDetected(void) 
{
  if (current == APPLICATION_FACEDETECT) 
		{
			return result_summ[1] > 0 ? true : false;//判断是否接收到数据
		}
  return false;
}

bool faceOfIdDetected(uint8_t id) 
{
  if (current == APPLICATION_FACEDETECT)
		{
			for (int i = 4; i < 4 + 29; ++i)
				{
					if (result_summ[i] == id)
						{
							return true;
						}
				}
		}
  return false;
}
/*返回指定ID的人脸*/
bool getFaceOfId(uint8_t id,struct   WonderCamFaceDetectResult *p)
	{
		memset(p, 0, sizeof(struct  WonderCamFaceDetectResult));
		if (current != APPLICATION_FACEDETECT)
		{
			return false;
		}
		for (int i = 4; i < 4 + 29; ++i)
		{
			if (result_summ[i] == id)
				{
					uint16_t index = i - 4;
					index = 0x30 + index * 16;
					readFromAddr(0x0400 + index, (void*)p, 16);
					return true;
				}
		}
  return false;
}
	
/*是否识别到了线*/
bool anyLineDetected(void) {
  if (current != APPLICATION_LINEFOLLOW) {
    return false;
  }
  return result_summ[1] > 0 ? true : false;
}
	
/*是否识别到了指定的线*/
bool lineIdDetected(uint8_t id) {
	int i,num;
  if (current != APPLICATION_LINEFOLLOW) {
    return false;
  }
   num = result_summ[1];
  for (i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      return true;
    }
  }
  return false;
}
	

/*获取指定的识别到的线的位置数据*/
bool lineId(uint8_t id, struct WonderCamLineResult *p) {
  memset(p, 0, sizeof(struct WonderCamLineResult));//
  if (current != APPLICATION_LINEFOLLOW) {
    return false;
  }
  if (!anyLineDetected()) {
    return false;
  }
  int num = result_summ[1];
  for (int i = 2; i < 2 + num; ++i) {
    if (result_summ[i] == id) {
      int r = readFromAddr(0x1400 + 48 + (16 * (i - 2)), (void*)p, 16);
      if (r != 16) {
        return false;
      }
      p->angle = p->angle > 90 ? p->angle - 180 : p->angle;
      p->offset = abs(p->offset) - 160;
      return true;
    }
  }
}

