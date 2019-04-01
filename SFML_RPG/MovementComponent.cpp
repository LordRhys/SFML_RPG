#include "MovementComponent.h"



MovementComponent::MovementComponent(sf::Sprite& sprite, float maxVelocity)
  : sprite(sprite), maxVelocity(maxVelocity)
{
  
}


MovementComponent::~MovementComponent()
{
}

// Accessors
const sf::Vector2f & MovementComponent::getVelocity() const
{
  return this->velocity;
}

// Functions
void MovementComponent::move(const float dir_x, const float dir_y, const float& dt)
{
  this->velocity.x = this->maxVelocity * dir_x;
  this->velocity.y = this->maxVelocity * dir_y;

  this->sprite.move(this->velocity * dt); // Uses Velocity
}

void MovementComponent::update(const float &dt)
{

}
