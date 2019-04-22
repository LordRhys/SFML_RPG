#include "Player.h"


// Initializer Functions
void Player::initVariables()
{
}

void Player::initComponents()
{
  
}

// Constructors / Destructors
Player::Player(float x, float y, sf::Texture& texture_sheet)
{
  this->initVariables();

  this->setPosition(x, y);

  this->createMovementComponent(300.0f, 15.0f, 5.0f);
  this->createAnimationComponent(texture_sheet);

  //this->animationComponent->addAnimation("IDLE_LEFT", 10.f, 0, 0, 7, 0, 32, 48);
  this->animationComponent->addAnimation("IDLE_LEFT", 10.f, 0, 0, 13, 0, 192, 192);
  this->animationComponent->addAnimation("WALK_LEFT", 10.f, 0, 1, 11, 1, 192, 192);
  this->animationComponent->addAnimation("WALK_RIGHT", 10.f, 0, 1, 11, 1, 192, 192);
}


Player::~Player()
{
}

// Functions
void Player::update(const float& dt)
{
  this->movementComponent->update(dt);

  if(this->movementComponent->getState(IDLE))
    this->animationComponent->play("IDLE_LEFT", dt);
  else if (this->movementComponent->getState(MOVING_LEFT))
    this->animationComponent->play("WALK_LEFT", dt);
  /*else if (this->movementComponent->getState(MOVING_RIGHT))
    this->animationComponent->play("WALK_RIGHT", dt);*/
}
