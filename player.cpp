#include "player.h"
#include "smartSprite.h"
#include "explodingMultiSprite.h"

Player::Player( const std::string& name) :
  MultiSprite(name),
  observers(),
  collision(false),
  explosion(nullptr),
  initialVelocity(getVelocity()),
  bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  activeBullets(),
  freeBullets(),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX") ),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastFrame(0)
{ }

Player::Player(const Player& s) :
  MultiSprite(s),
  observers(s.observers),
  collision(s.collision),
  explosion(s.explosion),
  initialVelocity(s.getVelocity()),
  bulletName(s.bulletName),
  activeBullets(s.activeBullets),
  freeBullets(s.freeBullets),
  minSpeed(s.minSpeed),
  bulletInterval(s.bulletInterval),
  timeSinceLastFrame(s.timeSinceLastFrame)
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

  for ( const Bullet& bullet : activeBullets ) {
    bullet.draw();
  }
}

void Player::shoot() {
  if ( timeSinceLastFrame < bulletInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
  // I need to add some minSpeed to velocity:
  Bullet bullet(bulletName);
  bullet.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
  bullet.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
  if (!freeBullets.empty()) {
    bullet = freeBullets.back();
    bullet.reset();
    bullet.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    bullet.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    activeBullets.push_back(bullet);
    freeBullets.pop_back();
  } else {
    activeBullets.push_back(bullet);
  }

  timeSinceLastFrame = 0;
}

void Player::update(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  auto iter = activeBullets.begin();
  for ( Bullet& bullet : activeBullets ) {
    if (bullet.goneTooFar()) {
      //activeBullets.erase(bullet);
      freeBullets.push_back(bullet);
      iter = activeBullets.erase(iter);
    } else {
      bullet.update(ticks);
      iter++;
    }
  }

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
