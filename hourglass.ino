#include <LedControl.h>
#include <MPU6050.h>
#include "hourglass.h"

/* Led matrix control pins. */
const int DIN = 5;
const int CS = 18;
const int CLK = 19;

/* Peripherals. */
MPU6050 mpu;
LedControl lc = LedControl(DIN, CLK, CS, 2);

/* Hourglass object. */
Hourglass *hourglass;

/* Particles mvoing time. */
unsigned long timeStart = 0;
unsigned long timeCurrent = 0;
unsigned long timeElapsed = 0;

/* Hourglass configutation. */
#define SQUARE_WIDTH        8
#define ACCEL_THRESHOLD     4

/* Hourglass pattern. */
uint8_t love_heart_pattern[SQUARE_WIDTH * SQUARE_WIDTH] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 0, 0, 0, 0, 0, 1,
  0, 1, 0, 0, 0, 0, 0, 1,
  0, 1, 0, 0, 0, 1, 1, 1,
  0, 1, 0, 0, 1, 0, 0, 0,
  0, 1, 0, 0, 1, 0, 0, 0,
  0, 0, 1, 1, 1, 0, 0, 0,
};

uint8_t one_minute_pattern[SQUARE_WIDTH * SQUARE_WIDTH] = {
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 0, 0,
  1, 1, 1, 1, 1, 1, 0, 0,
};

/* Show particles by top and bottom led matrix. */
void hourglassDisplay(int32_t squareNumber, uint8_t *squareData, int32_t width)
{
  byte value;
  int32_t row;
  int32_t col;

  for (row = 0; row < width; row++) {
    value = 0;
    for (col = 0; col < width; col++) {
      if (squareData[col * width + row] != 0) {
        value |= ((byte)0x01 << ((width - 1) - col));
      }
    }
    lc.setRow(squareNumber, row, value);
  }
}

void setup() {
  /* Initialization top matrix */
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);

  /* Initialization buttom matrix */
  lc.shutdown(1, false);
  lc.setIntensity(1, 5);
  lc.clearDisplay(1);

  /* Give a seed  */
  randomSeed(analogRead(0));

  /* Detect MPU6050 */
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    delay(500);
  }

  /* Create a hourglass */
  hourglass = new Hourglass(SQUARE_WIDTH, one_minute_pattern);

  /* Provide a display callback function to show particles in hourglass */
  hourglass->setDisplayEvent(hourglassDisplay);

  /* Show particles */
  hourglass->particlesDisplay();

  timeStart = millis();
}

void loop() {
  Vector normAccel;
  Direction direction = DIRECTION_DOWN;
  unsigned long interval;

  timeCurrent = millis();
  interval = timeCurrent - timeStart;

  /* Let particles move for each 50 millisecond */
  if (interval < 50) {
    return;
  }
  timeElapsed += interval;

  /* Reset time. */
  timeStart = timeCurrent;
  
  /* Detect hourglass direction by MPU6050. */
  normAccel = mpu.readNormalizeAccel();
  if (normAccel.XAxis >= ACCEL_THRESHOLD) {
    direction = DIRECTION_UP;
  } else if (normAccel.XAxis <= -ACCEL_THRESHOLD) {
    direction = DIRECTION_DOWN;
  } else if (normAccel.YAxis >= ACCEL_THRESHOLD) {
    direction = DIRECTION_LEFT;
  } else if (normAccel.YAxis <= -ACCEL_THRESHOLD) {
    direction = DIRECTION_RIGHT;
  }

  /* Set hourglass direction. */
  hourglass->setRotation(direction);

  /* Move particles according direction. */
  hourglass->particlesMove();

  /* Let particle fall through in another area for each 1 second. */
  if (timeElapsed >= 1000) {
    timeElapsed = 0;
    hourglass->particleFallThrough();
  }  
}
