#include <iostream>
#include <cmath>
#include "bullet.h"

void Bullet::update(Uint32 ticks) {
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
  if (tooFar) std::cout << "we gone too far fam" << std::endl;
}