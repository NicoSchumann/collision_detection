#ifndef BALL_HPP_
#define BALL_HPP_

#include <SFML/Graphics.hpp>

struct Ball : public sf::CircleShape
{
public:
	Ball( sf::Vector2<float> position, sf::Vector2<float> velVec, float radius);
	Ball( float xPos, float yPos, float horVel, float vertVel, float radius);

	/** Updates the coordinate for one move step */
	void move();

	/** True if this ball intersects with the other */
	bool intersects(const Ball & other) const;

	/** Velocity of the ball. */
	sf::Vector2<float> m_velVec;

	/** If the ball is in 'intersection' state. */
	bool m_intersection;
};


#endif /* BALL_HPP_ */
