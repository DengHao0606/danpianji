#include "Control.h"
#include "system.h"

struct WonderCamLineResult black_line;
extern int16_t line[2];
void Face_mode(void)
{
	updateResult();
	if (anyFaceDetected())//判断是否识别到人脸
		{ 				
			for (int i = 1; i < 6; i++)
				{
					if (faceOfIdDetected(i)) 
						{
							struct WonderCamFaceDetectResult p;//存放x,y坐标
							char str[20] = "ID:";
							str[4] = '\0';
							str[3] = 48 + i;
							getFaceOfId(i, &p);
							printf("%d,%d\n",p.x,p.y);
							HAL_Delay(200);
						}
				}
		}
}

void Line_mode(void)
{
	updateResult();
	char buf[100];
	if (lineIdDetected(1))
	{			
		if (lineId(1, &black_line)) {
//		printf("线条ID:1\r\n");
			line[0] = black_line.angle;
			line[1] = black_line.offset;
//		printf("起点X坐标:%d\r\n",black_line.start_x);
//		printf("起点Y坐标:%d\r\n",black_line.start_y);
//		printf("终点X坐标:%d\r\n",black_line.end_x);
//		printf("终点Y坐标:%d\r\n",black_line.end_y);
//		printf("线条角度:%d\r\n",black_line.angle);
//		printf("线条偏移:%d\r\n",black_line.offset);
		} 
	}
}