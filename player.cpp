#include "player.h"
#include "smartSprite.h"
#include "explodingMultiSprite.h"

Player::Player( const std::string& name) :
  MultiSprite(name),
  observers(),
  collision(false),
  explosion(nullptr),
  initialVelocity(getVelocity())
{ }

Player::Player(const Player& s) :
  MultiSprite(s),
  observers(s.observers),
  collision(s.collision),
  explosion(s.explosion),
  initialVelocity(s.getVelocity())
  { }

Player& Player::operator=(const Player& s) {
  MultiSprite::operator=(s);
  collision = s.collision;
  explosion = s.explosion;
  initialVelocity = s.initialVelocity;
  return *this;
}

void Player::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while (ptr != observers.end()) {
    if (*ptr == o) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

void Player::stop() {
  setVelocity(Vector2f(0,0));
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
  }
}
void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
  }
}
void Player::up()    {
  setPosition(Vector2f(getX(), 190));
}
void Player::down()  {
  setPosition(Vector2f(getX(), 280));

}

void Player::explode() {
  if ( !explosion ) explosion = new ExplodingMultiSprite(*this);
}

void Player::draw() const  {
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());

}

void Player::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while(ptr != observers.end() ) {
    (*ptr)->setPlayerPos( getPosition() );
    ++ptr;
  }

  stop();
}
