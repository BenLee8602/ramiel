#ifndef VECTORS_H_
#define VECTORS_H_


struct Vec2 {
	int x;
	int y;
};

struct Vec3 {
	int x;
	int y;
	int z;
};


struct Vec2f {
	float x;
	float y;
};

struct Vec3f {
	float x;
	float y;
	float z;
};


struct Matrix3 {
	float num[3][3] = { 0.0f };
};


#endif
