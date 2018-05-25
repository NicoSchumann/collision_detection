#include "Ball.hpp"
#include <cmath>

Ball::Ball(sf::Vector2<float> pos, sf::Vector2<float> velVec, float radius)
: m_velVec(velVec), m_intersection(false)
{
	setPosition(pos);
    setRadius(radius);
    setOrigin(radius,radius);
}

Ball::Ball(float xPos, float yPos, float horVel, float vertVel, float radius)
: m_intersection(false)
{
    setPosition(xPos, yPos);
    setRadius(radius);
    setOrigin(radius, radius);
    m_velVec.x = horVel;
    m_velVec.y = vertVel;
}

void
Ball::move()
{
	setPosition( getPosition() + m_velVec);
}

bool
Ball::intersects(const Ball & other) const
{
	sf::Vector2<float> distVec = getPosition() - other.getPosition();
	distVec.x = std::abs(distVec.x);
	distVec.y = std::abs(distVec.y);
	float dist = std::sqrt( distVec.x*distVec.x + distVec.y*distVec.y);

	if (dist <= getRadius() + other.getRadius())
	{
		return true;
	}
	return false;
}


