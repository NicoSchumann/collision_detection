
collision_detection: Ball.o Canvas.o GriddedCanvas.o main.o
	c++ -o collision_detection Ball.o Canvas.o GriddedCanvas.o main.o -lsfml-graphics -lsfml-system -lsfml-window
	
Ballo.o: Ball.hpp Ball.cpp
	c++ -c Ball.cpp
	
Canvas.o: Canvas.cpp Canvas.hpp
	c++ -c Canvas.cpp
	
GriddedCanvas.o: GriddedCanvas.cpp GriddedCanvas.hpp
	c++ -c GriddedCanvas.cpp

main.o: main.cpp Canvas.hpp GriddedCanvas.hpp
	c++ -c main.cpp

clean:
	rm -f collision_detection *.o
