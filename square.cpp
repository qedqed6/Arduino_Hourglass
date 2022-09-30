#include "square.h"

Square::Square(int32_t width, uint8_t *pattern)
{
  this->width = width;
  this->direction = DIRECTION_DOWN;

  this->size = width * width;
  this->data = new uint8_t[this->size];
  if (pattern == NULL) {
    memset(this->data, 0, this->size);
  } else {
    memcpy(this->data, pattern, this->size);
  }
}

Square::~Square()
{
  delete []data;
}

void Square::setRotation(Direction direction)
{
  this->direction = direction;
}

Vec2_t Square::getRotatedPosition(Vec2_t position)
{
  Vec2_t transform;

  switch (direction) {
  case DIRECTION_DOWN:
    /* Rotation 0 degree */
    transform.x = position.x;
    transform.y = position.y;
    break;
  case DIRECTION_RIGHT:
    /* Rotation 90 degree */
    transform.x = -position.y + (width - 1);
    transform.y = position.x;
    break;
  case DIRECTION_UP:
    /* Rotation 180 degree */
    transform.x = -position.x + (width - 1);
    transform.y = -position.y + (width - 1);
    break;
  case DIRECTION_LEFT:
    /* Rotation 270 degree */
    transform.x = position.y;
    transform.y = -position.x + (width - 1);
    break;
  default:
    transform.x = 0;
    transform.y = 0;
    break;
  }

  return transform;
}

uint8_t Square::getParticleState(Vec2_t position)
{
  Vec2_t transform;

  if ((position.x < 0) || (position.y < 0)) {
    return 0;
  }
  if ((position.x >= width) || (position.y >= width)) {
    return 0;
  }

  transform = getRotatedPosition(position);

  return data[transform.y * width + transform.x];
}

void Square::setParticleState(Vec2_t position, uint8_t state)
{
  Vec2_t transform;

  if ((position.x < 0) || (position.y < 0)) {
    return;
  }
  if ((position.x >= width) || (position.y >= width)) {
    return;
  }

  transform = getRotatedPosition(position);

  data[transform.y * width + transform.x] = state;
}

bool Square::allowParticleMoveDown(Vec2_t position)
{
  uint8_t state;

  if ((position.x < 1) || (position.y < 1)) {
    return false;
  }

  state = getParticleState(position);
  if (state == 0) {
    return false;
  }

  position.x -= 1;
  position.y -= 1;
  state = getParticleState(position);
  if (state == 0) {
    return true;
  }

  return false;
}

void Square::particleMoveDown(Vec2_t position)
{
  if (!allowParticleMoveDown(position)) {
    return;
  }

  /* Clear current particle */
  setParticleState(position, 0);

  /* Present particle in new position */
  position.x -= 1;
  position.y -= 1;
  setParticleState(position, 1);
}

bool Square::allowParticleMoveBottomLeft(Vec2_t position)
{
  uint8_t state;

  if (position.x < 1) {
    return false;
  }

  state = getParticleState(position);
  if (state == 0) {
    return false;
  }

  position.x -= 1;
  state = getParticleState(position);
  if (state == 0) {
    return true;
  }

  return false;
}

void Square::particleMoveBottomLeft(Vec2_t position)
{
  if (!allowParticleMoveBottomLeft(position)) {
    return;
  }

  /* Clear current particle */
  setParticleState(position, 0);

  /* Present particle in new position */
  position.x -= 1;
  setParticleState(position, 1);
}

bool Square::allowParticleMoveBottomRight(Vec2_t position)
{
  uint8_t state;

  if (position.y < 1) {
    return false;
  }

  state = getParticleState(position);
  if (state == 0) {
    return false;
  }
  
  position.y -= 1;
  state = getParticleState(position);
  if (state == 0) {
    return true;
  }

  return false;
}

void Square::particleMoveBottomRight(Vec2_t position)
{
  if (!allowParticleMoveBottomRight(position)) {
    return;
  }

  /* Clear current particle */
  setParticleState(position, 0);

  /* Present particle in new position */
  position.y -= 1;
  setParticleState(position, 1);
}

void Square::particleTryFalling(Vec2_t position)
{
  bool down = allowParticleMoveDown(position);
  bool right = allowParticleMoveBottomRight(position);
  bool left = allowParticleMoveBottomLeft(position);
  
  if (!down && !right && !left) {
    return;
  }

  if (down) {
    particleMoveDown(position);
    return;
  }

  /* Physic simulation through random direction. */
  if (left && right) {
    if (random(2) == 0) {
      particleMoveBottomRight(position);
    } else {
      particleMoveBottomLeft(position);
    }
    return;
  }

  if (right) {
    particleMoveBottomRight(position);
    return;
  }

  if (left) {
    particleMoveBottomLeft(position);
    return;
  }
}

uint8_t* Square::getData(void)
{
  return data;
}

int32_t Square::getWidth(void)
{
  return width;
}

void Square::particlesFalling(void)
{
  int32_t x;
  int32_t xdir;
  int32_t scan;
  uint8_t dir = random(2);
  Vec2_t particle;

  /* Buttom left triangle area */
  for (scan = 1; scan < width; scan++) {
    for (x = 0; x <= scan; x++) {
      xdir = (dir == 0) ? x : scan - x;
      particle.x = xdir;
      particle.y = -xdir + scan;
      particleTryFalling(particle);
    }
  }
  
  /* Top right triangle area */
  for (scan = 1; scan < width; scan++) {
    for (x = scan; x < width; x++) {
      xdir = (dir == 0) ? x : ((scan + width - 1) - x);
      particle.x = xdir;
      particle.y = -xdir + (scan + (width - 1));
      particleTryFalling(particle);
    }
  }
}