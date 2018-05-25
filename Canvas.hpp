#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include <SFML/Graphics.hpp>
#include "Ball.hpp"

class Canvas
{
public:
	Canvas();
	virtual ~Canvas();

	/** Handles the mouse events. */
	void handleInput();
	/** Updates the physical state. */
	virtual void update();
	/** Renders the physics. */
	void render();
	/** Throws a ball at the ground */
	Ball * spawnBall();
	/** Flags the end of the simulation */
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
