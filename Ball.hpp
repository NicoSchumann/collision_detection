/*
 * Ball.hpp
 *
 *  Created on: May 9, 2018
 *      Author: nico
 */

#ifndef BALL_HPP_
#define BALL_HPP_

#include <SFML/Graphics.hpp>

struct Ball : public sf::CircleShape
{
public:
	Ball( sf::Vector2<float> position, sf::Vector2<float> velVec, float radius);
	Ball( float xPos, float yPos, float horVel, float vertVel, float radius);

	void move();

	bool intersects(const Ball & other) const;

	sf::Vector2<float> m_velVec;
	bool m_intersection;
};


#endif /* BALL_HPP_ */
