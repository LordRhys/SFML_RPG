#include "Player.h"


// Initializer Functions
void Player::initVariables()
{
}

void Player::initComponents()
{
  this->createMovementComponent(100.0f);
}

// Constructors / Destructors
Player::Player(float x, float y, sf::Texture& texture)
{
  this->initVariables();
  this->initComponents();

  this->setTexture(texture);
  this->setPosition(x, y);
}


Player::~Player()
{
}
