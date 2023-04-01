game:
	g++ -std=c++17 main.cpp -o play -I include -L lib -l SDL2-2.0.0
	./play
	
linux:
	g++ -std=c++17 main.cpp -o play -lSDL2 -lSDL2_ttf

window:
	g++ window.cpp -o test -I include -L lib -l SDL2-2.0.0
	./test
