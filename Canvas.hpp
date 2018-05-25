/*
 * Canvas.hpp
 *
 *  Created on: May 9, 2018
 *      Author: nico
 */

#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include <SFML/Graphics.hpp>
#include "Ball.hpp"

class Canvas
{
public:
	Canvas();
	virtual ~Canvas();

	void handleInput();
	virtual void update();
	void render();
	Ball * spawnBall();
	bool isDone() const;

	sf::RenderWindow * getWindow();
protected:
	sf::RenderWindow * m_window;
	sf::Color m_background;
	bool m_done;
	float m_radius;  // ! Must be greater than velVec!
	int m_numberOfBalls;
	float m_maxVel;
	std::vector<Ball*> m_balls;
};

#endif /* CANVAS_HPP_ */
