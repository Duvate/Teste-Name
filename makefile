REL_DIR="Dependencies/__linux__/lib"

all: build clean run

build:
	cd src/Tetris && \
	g++ -std=c++11 -c *.cpp -I../../Dependencies/include -I../../Dependencies/__linux__/include -O3 && \
	g++ -std=c++11 *.o -o "Tetris" -L../../Dependencies/__linux__/lib -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lGL -ldl -O3

clean:
	cd src/Tetris && rm -f *.o

run:
	export LD_LIBRARY_PATH="$(PWD)/$(REL_DIR)" && \
	cd src/Tetris && \
	./Tetris