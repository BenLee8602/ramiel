build/libgraphicsbl.a: build/camera.o build/draw.o build/effects.o build/entity.o build/graphicsbl.o build/light.o build/physics.o build/triangle.o build/vec.o
	ar ru build/libgraphicsbl.a build/camera.o build/draw.o build/effects.o build/entity.o build/graphicsbl.o build/light.o build/physics.o build/triangle.o build/vec.o

build/camera.o: src/camera.h src/camera.cpp
	g++ -c src/camera.cpp -o build/camera.o

build/draw.o: src/draw.h src/draw.cpp
	g++ -c src/draw.cpp -o build/draw.o

build/effects.o: src/effects.h src/effects.cpp
	g++ -c src/effects.cpp -o build/effects.o

build/entity.o: src/entity.h src/entity.cpp
	g++ -c src/entity.cpp -o build/entity.o

build/graphicsbl.o: include/graphicsbl.h src/graphicsbl_p.h src/graphicsbl.cpp
	g++ -c src/graphicsbl.cpp -o build/graphicsbl.o

build/light.o: src/light.h src/light.cpp
	g++ -c src/light.cpp -o build/light.o

build/physics.o: src/physics.h src/physics.cpp
	g++ -c src/physics.cpp -o build/physics.o

build/triangle.o: src/triangle.h src/triangle.cpp
	g++ -c src/triangle.cpp -o build/triangle.o

build/vec.o: src/vec.h src/vec.cpp
	g++ -c src/vec.cpp -o build/vec.o
