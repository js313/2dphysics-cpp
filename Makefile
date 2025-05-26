build:
	g++ -std=c++17 -Wall ./src/*.cpp ./src/Physics/*.cpp -lm \
  	-I/opt/homebrew/include -D_THREAD_SAFE -lSDL2_image -lSDL2_gfx \
 	-L/opt/homebrew/lib -lSDL2 -o app

run:
	./app

build-run:
	make build && make run

clean:
	rm app
