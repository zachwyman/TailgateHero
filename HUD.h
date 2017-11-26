#ifndef HUD__H
#define HUD__H

#include <SDL.h>
#include <SDL_ttf.h>
#include "ioMod.h"


class HUD {
public:
  static HUD& getInstance();
  ~HUD();
  void draw() const;
private:
  SDL_Renderer* renderer;
  SDL_Rect rect;
  const IOmod& io;
  HUD();
};


#endif
