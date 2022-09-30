#ifndef SQUARE_H
#define SQUARE_H

#include <Arduino.h>
#include "vec.h"

enum Direction {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_RIGHT,
  DIRECTION_LEFT,  
};

class Square
{
  protected:
    Direction direction;
    int32_t width;

    uint8_t *data;
    int32_t size;

    Vec2_t getRotatedPosition(Vec2_t position);
    bool allowParticleMoveDown(Vec2_t position);
    bool allowParticleMoveBottomLeft(Vec2_t position);
    bool allowParticleMoveBottomRight(Vec2_t position);
    void particleMoveDown(Vec2_t position);
    void particleMoveBottomLeft(Vec2_t position);
    void particleMoveBottomRight(Vec2_t position);
    void particleTryFalling(Vec2_t position);

  public:
    /**
     * \brief Create a square with specified width.
     * @param width [in] Square's width.
     * @param pattern [in] An array of square data which contains a shape.
     */
    Square(int32_t width, uint8_t *pattern = NULL);

    ~Square();
    /**
     * \brief Get raw(non-rotated) square data.
     * @return width value.
     */
    uint8_t* getData(void);
    /**
     * \brief Get square width.
     * @return width value.
     */
    int32_t getWidth(void);
    /**
     * \brief Get particle state of specified position.
     * @return 0: Particle hiding, 1: Particle presenting.
     */
    uint8_t getParticleState(Vec2_t position);
    /**
     * \brief Set particle state of specified position.
     * @param state [in] Set 0: Particle hiding, 1: Particle presenting.
     */
    void setParticleState(Vec2_t position, uint8_t state);
    /**
     * \brief Rotation square to specified direction.
     * @param direction [in] accept up, buttom, right, left.
     */
    void setRotation(Direction direction);
    /**
     * \brief Let particles fall.
     */
    void particlesFalling(void);
};

#endif /* SQUARE_H */
