#ifndef TWOWAY__H
#define TWOWAY__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwoWay : public Drawable {
public:
  TwoWay(const std::string&);
  TwoWay(const TwoWay&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }

protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  TwoWay& operator=(const TwoWay&);
};
#endif