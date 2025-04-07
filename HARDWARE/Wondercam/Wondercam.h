#ifndef WONDER_CAM_H_
#define WONDER_CAM_H_
#include "System.h"

#define CAM_DEFAULT_I2C_ADDRESS       (0x32)
#pragma pack(1)
#define false 0
#define true 1
struct WonderCamQrCodeResultSumm {
  uint8_t current;
  uint8_t id;
  uint8_t __1[14];
  int16_t points[4][2];
  uint16_t len;
  uint8_t __2[14];
};

struct WonderCamFaceDetectResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
} __attribute__((aligned(16)));

struct WonderCamObjDetectResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
} __attribute__((aligned(16)));

struct WonderCamColorDetectResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
} __attribute__((aligned(16)));

struct WonderCamLineResult {
  int16_t start_x;
  int16_t start_y;
  int16_t end_x;
  int16_t end_y;
  int16_t angle;
  int16_t offset;
} __attribute__((aligned(16)));

struct WonderCamAprilTagResult {
  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
  float x_t;
  float x_r;
  float y_t;
  float y_r;
  float z_t;
  float z_r;
} __attribute__((aligned(32)));

#pragma pack()


#define WONDERCAM_LED_ON           (true)
#define WONDERCAM_LED_OFF          (false)


typedef enum {
  APPLICATION_NONE = 0,
  APPLICATION_FACEDETECT,
  APPLICATION_OBJDETECT,
  APPLICATION_CLASSIFICATION,
  APPLICATION_FEATURELEARNING,
  APPLICATION_COLORDETECT,
  APPLICATION_LINEFOLLOW,
  APPLICATION_APRILTAG,
  APPLICATION_QRCODE,
  APPLICATION_BARCODE,
  APPLICATION_MAX,
} APPLICATION;

typedef enum {
  Aeroplane = 1,
  Bicycle,
  Bird,
  Boat,
  Bottle,
  Bus,
  Car,
  Cat,
  Chair,
  Cow,
  Diningtable,
  Dog,
  Horse,
  Motorbike,
  Person,
  Pottedplant,
  Sheep,
  Sofa,
  Train,
  Monitor
}Objects;

int writeToAddr(uint16_t addr, const uint8_t *buf, uint16_t leng);
int readFromAddr(uint16_t addr,  uint8_t *buf, uint16_t leng);
int currentFunc(void);
int readByte(uint16_t address);
bool changeFunc(uint8_t new_func);
bool updateResult(void);
bool anyFaceDetected(void);
bool faceOfIdDetected(uint8_t id);
bool getFaceOfId(uint8_t id,struct   WonderCamFaceDetectResult *p);
void setLed(bool new_state); 
//巡线
bool anyLineDetected(void);
int numOfLineDetected(void);
bool lineIdDetected(uint8_t id);
bool lineId(uint8_t id, struct WonderCamLineResult *p);
#endif
