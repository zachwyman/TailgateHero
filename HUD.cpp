#include "HUD.h"
#include "gamedata.h"
#include "renderContext.h"

HUD& HUD::getInstance() {
  static HUD instance;
  return instance;
}

HUD::~HUD() {

}

HUD::HUD() :
  renderer( RenderContext::getInstance()->getRenderer() ),
  rect({  Gamedata::getInstance().getXmlInt("HUD/x"),
          Gamedata::getInstance().getXmlInt("HUD/y"),
          Gamedata::getInstance().getXmlInt("HUD/width"),
          Gamedata::getInstance().getXmlInt("HUD/height")
        }),
  io( IOmod::getInstance() )
{

}

void HUD::draw() const {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255/2 );
  SDL_RenderFillRect ( renderer, &rect );
  SDL_RenderDrawRect( renderer, &rect );
  io.writeText("Instructions for Tailgate Hero: ", {255, 255, 255, 255},
    Gamedata::getInstance().getXmlInt("HUD/x")+10, Gamedata::getInstance().getXmlInt("HUD/y")+10);
  io.writeText("Use D to drive the truck forward ", {255, 255, 255, 255},
    Gamedata::getInstance().getXmlInt("HUD/x")+10, Gamedata::getInstance().getXmlInt("HUD/y")+40);
  io.writeText("and A to drive the car in reverse ", {255, 255, 255, 255},
    Gamedata::getInstance().getXmlInt("HUD/x")+10, Gamedata::getInstance().getXmlInt("HUD/y")+70);
  io.writeText("Use W and S to switch lanes ", {255, 255, 255, 255},
    Gamedata::getInstance().getXmlInt("HUD/x")+10, Gamedata::getInstance().getXmlInt("HUD/y")+100);
  io.writeText("Press F1 to hide/show HUD ", {255, 255, 255, 255},
    Gamedata::getInstance().getXmlInt("HUD/x")+10, Gamedata::getInstance().getXmlInt("HUD/y")+130);
}
