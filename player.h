#ifndef PLAYER__H
#define PLAYER__H

#include <list>
#include "multisprite.h"
#include "smartSprite.h"

class ExplodingMultiSprite;

class Player : public MultiSprite {
public:
  Player(const std::string&);
  Player(const Player&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);

  void attach( SmartSprite* o ) { observers.push_back(o); }
  void detach( SmartSprite* o );
  

  void right();
  void left();
  void up();
  void down();
  void stop();

  virtual void explode();
private:
  std::list<SmartSprite*> observers;
  bool collision;
  ExplodingMultiSprite* explosion;
  Vector2f initialVelocity;
};
#endif
