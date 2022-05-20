build/libgraphicsbl.a: build/camera.o build/draw.o build/effects.o build/entity.o build/graphicsbl.o build/light.o build/model.o build/objloader.o build/physics.o build/texture.o build/triangle.o build/vec.o
	ar ru build/libgraphicsbl.a build/camera.o build/draw.o build/effects.o build/entity.o build/graphicsbl.o build/light.o build/model.o build/objloader.o build/physics.o build/texture.o build/triangle.o build/vec.o

build/camera.o: src/camera.h src/camera.cpp
	g++ -Ideps -c -O3 src/camera.cpp -o build/camera.o

build/draw.o: src/draw.h src/draw.cpp
	g++ -Ideps -c -O3 src/draw.cpp -o build/draw.o

build/effects.o: src/effects.h src/effects.cpp
	g++ -Ideps -c -O3 src/effects.cpp -o build/effects.o

build/entity.o: src/entity.h src/entity.cpp src/objloader.h
	g++ -Ideps -c -O3 src/entity.cpp -o build/entity.o

build/graphicsbl.o: include/graphicsbl.h src/graphicsbl_p.h src/graphicsbl.cpp
	g++ -Ideps -c -O3 src/graphicsbl.cpp -o build/graphicsbl.o

build/light.o: src/light.h src/light.cpp
	g++ -Ideps -c -O3 src/light.cpp -o build/light.o

build/model.o: src/model.h src/model.cpp
	g++ -Ideps -c -O3 src/model.cpp -o build/model.o

build/objloader.o: src/objloader.h src/objloader.cpp
	g++ -Ideps -c -O3 src/objloader.cpp -o build/objloader.o

build/physics.o: src/physics.h src/physics.cpp
	g++ -Ideps -c -O3 src/physics.cpp -o build/physics.o

build/texture.o: src/texture.h src/texture.cpp
	g++ -Ideps -c -O3 src/texture.cpp -o build/texture.o

build/triangle.o: src/triangle.h
	g++ -Ideps -c -O3 src/triangle.h -o build/triangle.o

build/vec.o: src/vec.h src/vec.cpp
	g++ -Ideps -c -O3 src/vec.cpp -o build/vec.o
