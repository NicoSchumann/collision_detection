#include "GriddedCanvas.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <exception>
#include <array>


GriddedCanvas::GriddedCanvas()
:Canvas::Canvas()
{
	// std::cerr << "GC1 "; // debug
	auto ws = m_window->getSize();

	// Set the sectors size.
	for (int i = 0; i < 8; ++ i) // row
	{
		for(int k = 0; k < 8; ++ k) // column
		{
			Sector sector;
			sector.width  = static_cast<float>(ws.x)/8;
			sector.height = static_cast<float>(ws.y)/8;
			sector.left =  static_cast<float>(k) * ws.x / 8;
			sector.top  =  static_cast<float>(i) * ws.y / 8;
			m_sectors[i*8 + k] = sector;
		}
	}
	/*// debug
	int i = 0;
	for (auto & s : m_sectors)
	{
		std::cerr << "sector_"<<i<<": ("<< s.left <<":"<< s.top <<"):("<< s.width << ":"<< s.height <<")\n";
		++i;
	}
	*/ // debug
    // std::cerr << " GC2 "; // debug

	// Store a pointer of each ball in its matching sector.
	for (Ball * ballPtr : m_balls)
	{
		//std::cerr << " GC3:"<< ballPtr->getPosition().x << ','<< ballPtr->getPosition().y; // debug
		int sectorNo = getSectorNo(*ballPtr);
		//std::cerr << " GC4:SectorNo:" << sectorNo; // debug
		m_sectors[sectorNo].m_ballList.push_front(ballPtr);
		//std::cerr << " GC10 "; // debug
	}
	//std::cerr << "GriddedCanvas() "; // debug
	//for (auto & ballPtr : m_balls) std::cerr << ballPtr->m_velVec.x << ',' << ballPtr->m_velVec.y << ' '; // debug
	//std::cerr << '\n'; // debug
}
int
GriddedCanvas::getSectorNo(const Ball & ball) const
{
	for (int i = 0; i < static_cast<int>(m_sectors.size()); ++i)
	{
		if (sectorContains(ball, i))
		{
			return i;
		}
	}
	sf::Vector2<float> pos = ball.getPosition();
	std::cerr << "getSectorNo(): " << pos.x << "," << pos.y << ")\n";
	throw std::logic_error("In getSectorNo(): no matching sector.\n");
}
int
GriddedCanvas::getSectorNo(const Ball & ball, int sectorNo) const
{
	// Evaluate all adjacent sectors relative to 'sectorNo':
	std::vector<int> adjacentSectors;
	// The upper sectors:
	if ( sectorNo > 7) {
		adjacentSectors.push_back(sectorNo - 8); // upper
		if (sectorNo % 8 != 0) {
			adjacentSectors.push_back(sectorNo - 9); // upper left
		} if (sectorNo % 8 != 7) {
			adjacentSectors.push_back(sectorNo - 7); // upper right
		}
	}
	// The lower sectors:
	if (sectorNo < 56) {
		adjacentSectors.push_back(sectorNo + 8); // lower
		if( sectorNo % 8 != 0) {
			adjacentSectors.push_back(sectorNo + 7); // lower left
		} if (sectorNo % 8 != 7) {
			adjacentSectors.push_back(sectorNo + 9); // lower right
		}
	}
	// Left- and right-sided sectors:
	if (sectorNo % 8 != 0) {
		adjacentSectors.push_back(sectorNo - 1); // left
	} if (sectorNo % 8 != 7) {
		adjacentSectors.push_back(sectorNo + 1); // right
	}

	// Test the adjacent sectors for finding the ball:
	for(auto sNo : adjacentSectors)
	{
		if (sectorContains(ball, sNo))
		{
			return sNo;
		}
	}
	// If ball not found, throw exception:
	sf::Vector2<float> pos = ball.getPosition();
	std::cerr << "getSectorNo(Ball&, int): ball("<<pos.x<<","<<pos.y<<") is not adjacent to sector "<<sectorNo<<".\n";
	throw std::logic_error("Thrown in getSectorNo(Ball&,int).");
}
bool
GriddedCanvas::sectorContains(const Ball & ball, int sectorNo) const
{
	if (m_sectors.at(sectorNo).contains(ball.getPosition()))
	{
		return true;
	}
	return false;
}

void
GriddedCanvas::update()
{

	for (std::size_t sectorNo = 0; sectorNo < m_sectors.size(); ++ sectorNo)
	{
		updateSector(sectorNo);
	}
}

void
GriddedCanvas::updateSector( int sectorNo)
{
		for (auto it = m_sectors[sectorNo].m_ballList.begin(); it != m_sectors[sectorNo].m_ballList.end(); )
		{
			Ball & ball = **it;
			ball.move();

			// Handle window borders.
			auto ws = m_window->getSize();
		    // upper border
		    if (sectorNo < 8){
		    	for ( Ball * ballPtr : m_sectors[sectorNo].m_ballList)
		    	{
		    		if (ballPtr->getPosition().y -m_radius <= 0)
		    		{
		    			ballPtr->m_velVec.y = - ballPtr->m_velVec.y;
		    			auto pos = ballPtr->getPosition();
		    			ballPtr->setPosition(pos.x, m_radius);
		    			//std::cerr << "Bounced from top.\n"; // debug
		    		}
		    	}
		    }  // lower boarder
		    else if (sectorNo > 55) {
		    	for ( Ball * ballPtr : m_sectors[sectorNo].m_ballList)
		    	{
		    		if (ballPtr->getPosition().y + m_radius >= ws.y)
		    		{
		    			ballPtr->m_velVec.y = - ballPtr->m_velVec.y;
		    			auto pos = ballPtr->getPosition();
		    			ballPtr->setPosition(pos.x, ws.y-m_radius);
		    			//std::cerr << "Bounced from bottom.\n"; // debug
		    		}
		    	}
		    }
		    // left border
		    if (sectorNo % 8 == 0) {
		    	for ( Ball * ballPtr : m_sectors[sectorNo].m_ballList)
		    	{
		    		if (ballPtr->getPosition().x - m_radius <= 0)
		    		{
		    			ballPtr->m_velVec.x = - ballPtr->m_velVec.x;
		    			auto pos = ballPtr->getPosition();
		    			ballPtr->setPosition( m_radius, pos.y);
		    			//std::cerr << "Bounced from left.\n"; // debug
		    		}
		    	}
		    }  // right border
		    else if (sectorNo % 8 == 7) {
		    	for ( Ball * ballPtr : m_sectors[sectorNo].m_ballList)
		    	{
		    		if (ballPtr->getPosition().x + m_radius >= ws.x)
		    		{
		    			ballPtr->m_velVec.x = - ballPtr->m_velVec.x;
		    			auto pos = ballPtr->getPosition();
		    			ballPtr->setPosition(ws.x-m_radius, pos.y);
		    			//std::cerr << "Bounced from right.\n"; // debug
		    		}
		    	}
		    }

			// Ensure that the ball belongs to the correct sector.
			if (!sectorContains(ball, sectorNo))
			{
				int sNo = getSectorNo(ball); // A very expensive operation, should optimized later.
				if (sNo < 0 || sNo > 63) std::cerr << "sNo was " << sNo << ".\n"; // debug

				m_sectors[sNo].m_ballList.push_front(&ball);

				//std::cerr << "m_sectors["<<sNo<<"].m_ballList grows. New size:" << m_sectors[sectorNo].m_ballList.size() << "\n"; //debug
				it = m_sectors[sectorNo].m_ballList.erase(it);  // 'it' will automatically incremented.
				//std::cerr << "m_sectors["<<sectorNo<<"].m_ballList shrinks. New size:"<< m_sectors[sectorNo].m_ballList.size() << "\n"; //debug
				continue;
			}
			else
			{
				++it;
			}


			// Handle intersection.
			for (auto ito = m_sectors[sectorNo].m_ballList.begin(); ito != m_sectors[sectorNo].m_ballList.end(); ++ito)
			{
				Ball & other = **ito;
				if (ball.intersects(other))
				{
					sf::Vector2<float> tmp = ball.m_velVec;
					ball.m_velVec = other.m_velVec;
					other.m_velVec = tmp;
				}
			}
		}


		// Handle intersection of balls from adjacent sectors
		//   -|---|---|---|-
		//    |   |   |   |   upper-left upper upper-right
		//   -|---|---|---|-
		//    |   | s |   |   left   [this sector]   right
		//   -|---|---|---|-
		//    |   |   |   |   lower-left lower lower-right
		//   -|---|---|---|-
		//
        // We must take care about all sectors which align to the window borders.

		std::vector<int> adjacentSectors;
		// The upper sectors:
		if ( sectorNo > 7) {
			adjacentSectors.push_back(sectorNo - 8); // upper
			if (sectorNo % 8 != 0) {
				adjacentSectors.push_back(sectorNo - 9); // upper left
			} if (sectorNo % 8 != 7) {
				adjacentSectors.push_back(sectorNo - 7); // upper right
			}
		}
		// The lower sectors:
		if (sectorNo < 56) {
			adjacentSectors.push_back(sectorNo + 8); // lower
		    if( sectorNo % 8 != 0) {
		    	adjacentSectors.push_back(sectorNo + 7); // lower left
		    } if (sectorNo % 8 != 7) {
		    	adjacentSectors.push_back(sectorNo + 9); // lower right
		    }
		}
		// Left- and right-sided sectors:
		if (sectorNo % 8 != 0) {
			adjacentSectors.push_back(sectorNo - 1); // left
		} if (sectorNo % 8 != 7) {
			adjacentSectors.push_back(sectorNo + 1); // right
		}

		for (Ball * ballPtr : m_sectors.at(sectorNo).m_ballList)
		{
			Ball & ball = *ballPtr;
		    for (int secNo : adjacentSectors)
		    {
			    for( Ball * otherPtr : m_sectors.at(secNo).m_ballList)
			    {
			    	Ball & other = *otherPtr;
				    if (ball.intersects(other))
				    {
					    sf::Vector2<float> tmp = ball.m_velVec;
					    ball.m_velVec = other.m_velVec;
					    other.m_velVec = tmp;
				    }
				}
			}
		}
}

