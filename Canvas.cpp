/*
 * Canvas.cpp
 *
 *  Created on: May 9, 2018
 *      Author: nico
 */

#include "Canvas.hpp"
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <iostream>


Canvas::Canvas()
:m_background(sf::Color::Black)
,m_done(false),m_radius(5),m_numberOfBalls(2000),m_maxVel(5)
{
	m_balls.reserve(m_numberOfBalls);
	m_window = new sf::RenderWindow(sf::VideoMode(1200,800),"collision_detection");
	std::srand(time(0));

	for (int i = 0; i < m_numberOfBalls; ++i)
	{
		Ball * ballPtr = spawnBall();
		bool intersectionFlag;
		do {
			intersectionFlag = false;
		    for (auto other : m_balls)
		    {
		    	if (ballPtr->intersects(*other))
		    	{
		    		intersectionFlag = true;
		    		sf::Vector2u ws = m_window->getSize();
				    float xPos = float(rand() % (ws.x - int(4*m_radius))) + 2*m_radius;
				    float yPos = float(rand() % (ws.y - int(4*m_radius))) + 2*m_radius;
				    ballPtr->setPosition(xPos, yPos);
		    	}
		    }
		} while (intersectionFlag);

		m_balls.push_back(ballPtr);
	}
}

Canvas::~Canvas()
{
	delete m_window;
	for (auto & ballPtr : m_balls)
	{
		delete ballPtr;
	}
}

bool
Canvas::isDone() const
{
	return m_done;
}

void
Canvas::handleInput()
{
    sf::Event event;
	while (m_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_done = true;
		}
	}
}
void
Canvas::render()
{
	m_window->clear(m_background);
	for ( auto & ballPtr : m_balls)
	{
		m_window->draw(*ballPtr);
	}
	m_window->display();
}
Ball *
Canvas::spawnBall()
{
	sf::Vector2u windowSize = m_window->getSize();
	float xPos    = float(rand() % (windowSize.x - int(4*m_radius))) + 2*m_radius;
	float yPos    = float(rand() % (windowSize.y - int(4*m_radius))) + 2*m_radius;

	// ! vertVel and horVel must be lower than m_radius!
	float vertVel = rand() % (int)(m_maxVel * 100) / 100.0f;
	float horVel  = rand() % (int)(m_maxVel * 100) / 100.0f;

    Ball * ball = new Ball(xPos, yPos, vertVel, horVel, m_radius);
    ball->setPointCount(6);
    ball->setOrigin(m_radius, m_radius);
    ball->setFillColor(sf::Color(sf::Color::White));
    return ball;
}
void
Canvas::update()
{
	for (auto it = m_balls.begin(); it != m_balls.end(); ++it)
	{
		Ball & ball = **it;
		ball.move();

		for (auto ito = m_balls.begin(); ito != m_balls.end(); ++ito)
		{
			// Nothing happens if the above line will be commented out,
			// rather it saves CPU time.
			// if (*it == *ito) continue;  // If both iterators point to the same object.

			Ball & other = **ito;
			if (ball.intersects(other))
			{
				auto tmpVec =  ball.m_velVec;
				ball.m_velVec = other.m_velVec;
				other.m_velVec = tmpVec;
			}
		}

		sf::Vector2<float> pos = ball.getPosition();
		sf::Vector2u ws = m_window->getSize();

		/*
		// If window frame is not bounding:
		ball.setPosition(static_cast<int>(pos.x) % ws.x, static_cast<int>(pos.y) % ws.y);
        */

		// If window frame is bounding:
		float radius = ball.getRadius();
		if (pos.x + radius >= ws.x || pos.x - radius <= 0)
	    {
			ball.m_velVec.x = - ball.m_velVec.x;
	    }
		if (pos.y + radius >= ws.y || pos.y - radius <= 0)
		{
			ball.m_velVec.y = - ball.m_velVec.y;
		}

	}
	// std::this_thread::sleep_for(std::chrono::milliseconds(20));
}
