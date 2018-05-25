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

	/** Updates the physical state. */
	void update();
private:
	/** Updates a distinct sector of the canvas. */
	void updateSector(int sectorNo);

	/** Returns a ball's sector number. **/
	int getSectorNo(const Ball & ball) const;

	/** Returns a ball's sector number of a neighbored sector. */
	int getSectorNo(const Ball & ball, int oldSectorNo) const;

	/** True if ball is within the sector */
	bool sectorContains(const Ball & ball, int sectorNo) const;

	std::array<Sector,64> m_sectors;
};


#endif /* GRIDDEDCANVAS_HPP_ */
