#include "hourglass.h"

Hourglass::Hourglass(int32_t squareWidth, uint8_t *pattern)
{
  width = squareWidth;
  displayEvent = NULL;
  topSquare = new Square(squareWidth, pattern);
  buttomSquare = new Square(squareWidth);

  setRotation(DIRECTION_DOWN);
}

Hourglass::~Hourglass()
{
  delete topSquare;
  delete buttomSquare;
}

void Hourglass::setDisplayEvent(HourglassDisplayEvent event)
{
  displayEvent = event;  
}

void Hourglass::setRotation(Direction direction)
{
  this->direction = direction;
  topSquare->setRotation(direction);
  buttomSquare->setRotation(direction);
}

void Hourglass::particlesDisplay(void)
{
  if (displayEvent != NULL) {
    displayEvent(0, topSquare->getData(), topSquare->getWidth());
    displayEvent(1, buttomSquare->getData(), buttomSquare->getWidth());
  }
}

void Hourglass::particleFallThrough(void)
{
  uint8_t topState;
  uint8_t buttomState;

  if (direction == DIRECTION_DOWN) {    
    topState = topSquare->getParticleState(Vec2_t(0, 0));      
    buttomState = buttomSquare->getParticleState(Vec2_t(width - 1, width - 1));

    if (topState != 0 && buttomState == 0) {
      topSquare->setParticleState(Vec2_t(0, 0), 0);
      buttomSquare->setParticleState(Vec2_t(width - 1, width - 1), 1);
      particlesDisplay();
    }
    return;
  }

  if (direction == DIRECTION_UP) {
    topState = topSquare->getParticleState(Vec2_t(width - 1, width - 1));      
    buttomState = buttomSquare->getParticleState(Vec2_t(0, 0));

    if (topState == 0 && buttomState != 0) {
      topSquare->setParticleState(Vec2_t(width - 1, width - 1), 1);
      buttomSquare->setParticleState(Vec2_t(0, 0), 0);
      particlesDisplay();
    }
    return;
  }
}

void Hourglass::particlesMove(void)
{
  topSquare->particlesFalling();
  buttomSquare->particlesFalling();
  particlesDisplay();
}