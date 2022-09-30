#ifndef HOURGLASS_H
#define HOURGLASS_H

#include <Arduino.h>
#include "vec.h"
#include "square.h"

/**
 * \brief Display event
 * @param squareNumber Tell which square' particles be updated.
 * @param squareData Get raw(non-rotated) square data.
 * @param width Square width.
 */
typedef void (*HourglassDisplayEvent)(int32_t squareNumber, uint8_t *squareData, int32_t width);

class Hourglass
{
  protected:
    Direction direction;
    int32_t width;

    Square *topSquare;
    Square *buttomSquare;

    HourglassDisplayEvent displayEvent;

  public:
    /**
     * \brief Create a hourglass with specified square width.
     * @param width [in] Each Square's width.
     * @param pattern [in] An array of top square data which contains a shape.
     */
    Hourglass(int32_t squareWidth, uint8_t *pattern = NULL);

    ~Hourglass();
    /**
     * \brief Set a event to track particles state.
     * @param event [in] Envent callback function
     */
    void setDisplayEvent(HourglassDisplayEvent event);
    /**
     * \brief Force trigger display event to show current particles state.
     */
    void particlesDisplay(void);
    /**
     * \brief Rotation hourglass to specified direction.
     * @param direction [in] accept up, buttom, right, left.
     */
    void setRotation(Direction direction);
    /**
     * \brief Move particles and trigger display event.
     */
    void particlesMove(void);
    /**
     * \brief Move particles through to another area.
     */
    void particleFallThrough(void);
};

#endif /* HOURGLASS_H */
