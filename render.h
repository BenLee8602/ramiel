#ifndef RENDER_H_
#define RENDER_H_


struct Vec2 {
	int x;
	int y;
};

struct Vec3 {
	int x;
	int y;
	int z;
};


struct Tri2D {
	struct Vec2 pts[3];
};

struct Tri3D {
	struct Vec3 pts[3];
};


struct Vec2 convDimension(struct Vec3* pt3D);
void renderMain();


#endif
