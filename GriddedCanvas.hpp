/*
 * GriddedCanvas.hpp
 *
 *  Created on: May 19, 2018
 *      Author: nico
 */

#ifndef GRIDDEDCANVAS_HPP_
#define GRIDDEDCANVAS_HPP_

#include "Canvas.hpp"
#include <array>
#include <list>

struct Sector : public sf::Rect<float>
{
	std::list<Ball*> m_ballList;
};

class GriddedCanvas : public Canvas
{
public:
	GriddedCanvas();
	void update();
private:
	void updateSector(int sectorNo);
	int getSectorNo(const Ball & ball) const;
	int getSectorNo(const Ball & ball, int oldSectorNo) const;
	bool sectorContains(const Ball & ball, int sector) const;
	std::array<Sector,64> m_sectors;
};


#endif /* GRIDDEDCANVAS_HPP_ */
