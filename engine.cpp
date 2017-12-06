#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twoway.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "player.h"
#include "collisionStrategy.h"

Engine::~Engine() {
  for (unsigned int i = 0; i < sprites.size(); i++) {
    delete sprites[i];
  }

  for (unsigned int i = 0; i < strategies.size(); i++) {
    delete strategies[i];
  }
  delete player;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  hud( HUD::getInstance() ),
  hudFlag( false ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  nightsky("nightsky", Gamedata::getInstance().getXmlInt("nightsky/factor") ),
  stands("stands", Gamedata::getInstance().getXmlInt("stands/factor") ),
  field("field", Gamedata::getInstance().getXmlInt("field/factor") ),
  viewport( Viewport::getInstance() ),
  player(new Player("Truck")),
  sprites(),
  strategies(),
  currentStrategy(0),
  collision( false),
  makeVideo( false )
{

  for (int i = 0; i < 50; i++) {
    sprites.push_back(new Sprite("Bomb"));
  }

  strategies.push_back(new PerPixelCollisionStrategy );



  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  nightsky.draw();
  stands.draw();
  field.draw();

  for (unsigned int i = 0; i < sprites.size(); i++) {
    sprites[i]->draw();
  }

  if (clock.getSeconds() < 3) {
    hud.draw();
    std::stringstream strm;
    strm << "Zachary Wyman's game\n fps: " << clock.getFps();
    io.writeText(strm.str(), {255, 0, 255, 255}, 30, 440);
  }
  else if (hudFlag) {
    hud.draw();
    std::stringstream strm;
    strm << "Zachary Wyman's game\n fps: " << clock.getFps();
    io.writeText(strm.str(), {255, 0, 255, 255}, 30, 440);
  }
//  strategies[currentStrategy]->draw();
  if(collision) {
    io.writeText("COLLISION!!!!", {0,0,0,255}, 250, 250);
  }

  player->draw();
  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  collision = false;
  for ( Drawable* d : sprites ) {
    if ( strategies[currentStrategy]->execute(*player, *d) ) {
      collision = true;
      static_cast<Sprite*>(d)->explode();
    }
  }
  if ( collision ) {
    player->explode();
  }
  else {
    player->missed();
    collision = false;
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  player->update(ticks);
  for (unsigned int i = 0; i < sprites.size(); i++) {
    sprites[i]->update(ticks);
  }

  nightsky.update();
  stands.update();
  field.update();
  viewport.update(); // always update viewport last
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
        if (keystate[SDL_SCANCODE_F1]) {
          hudFlag = !hudFlag;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    // for (unsigned int i = 0; i < sprites.size(); i++) {
    //   if (static_cast<Sprite*>(sprites[i])->getExplosions()) {
    //     sprites.erase(sprites.begin() + i);
    //     delete sprites[i];
    //   }
    // }

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        player->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        player->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        player->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        player->down();
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
