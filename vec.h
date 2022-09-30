#ifndef VEC_H
#define VEC_H

#include <Arduino.h>

typedef struct Vec2 {
  int32_t x;
  int32_t y;

  Vec2(int32_t x = 0, int32_t y = 0)
  {
    this->x = x;
    this->y = y;
  }
} Vec2_t;

#endif /* VEC_H */
