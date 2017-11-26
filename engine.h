#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "HUD.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Player;
class CollisionStrategy;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IOmod& io;
  const HUD& hud;
  bool hudFlag;
  Clock& clock;

  SDL_Renderer * const renderer;
  World nightsky;
  World stands;
  World field;
  Viewport& viewport;

  Player* player;
  std::vector<Drawable*> sprites;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  bool collision;

  bool makeVideo;

  void draw() const;
  void update(Uint32);

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  void printScales() const;
  void checkForCollisions();
};
