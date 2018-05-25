/*
 * main.cpp
 *
 *  Created on: May 9, 2018
 *      Author: nico
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include "GriddedCanvas.hpp"

int main() {

	GriddedCanvas canvas;
	while (!canvas.isDone())
	{
		canvas.handleInput();
		canvas.update();
		canvas.render();
	}
	return 0;
}

