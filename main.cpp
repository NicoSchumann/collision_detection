#include <SFML/Graphics.hpp>
#include <iostream>
#include "GriddedCanvas.hpp"

int main() {

	// Canvas canvas;
	GriddedCanvas canvas;
	while (!canvas.isDone())
	{
		canvas.handleInput();
		canvas.update();
		canvas.render();
	}
	return 0;
}

