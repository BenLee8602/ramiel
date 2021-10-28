#include "render.h"
#include <Windows.h>
namespace bl {

	std::vector<Entity> RenderBL::entities;
	Vec3f RenderBL::light_ambient = vec3f_0;
	std::vector<Light*> RenderBL::lights;
	int* RenderBL::pixels;
	float* RenderBL::depth;
	Camera RenderBL::cam;
	Vec2 RenderBL::size;
	Vec2 RenderBL::mid;
	int RenderBL::bufferSize;
	float RenderBL::dtime;
	const float RenderBL::znear = 0.2f;
	const float RenderBL::zfar = 1000.0f;
	std::ofstream RenderBL::debug("output.txt");


	void RenderBL::setBuffer(void* pixelBuffer, int sizex, int sizey) {
		pixels = (int*)pixelBuffer;
		size[x] = sizex;
		size[y] = sizey;
		mid[x] = sizex / 2;
		mid[y] = sizey / 2;
		bufferSize = sizex * sizey;
		delete[] depth;
		depth = new float[sizex * sizey];
		setFov(90);
	}


	void RenderBL::setFov(int fov) {
		cam.setFov(fov);
	}


	void RenderBL::renderFrame(float dtime) {
		std::fill(pixels, pixels + bufferSize, 0);
		std::fill(depth, depth + bufferSize, zfar);
		RenderBL::dtime = dtime;
		cam.getControls();
		cam.calcTrigValues();
		lights[0]->move(cam.getpos()); // temp
		for (auto& e : entities) {
			e.draw();
		}
	}


	void RenderBL::addEntity(const char* objfilename, char shading, float _x, float _y, float _z, float _r, float _g, float _b) {
		Vec3f pos = { _x, _y, _z };
		Vec3f color = { _r, _g, _b };
		if (!_r && !_g && !_b) color = vec3f_255;
		entities.push_back(Entity(objfilename, pos, color, shading));
	}


	void RenderBL::addLight(char type, float _x, float _y, float _z, float _r, float _g, float _b, float falloff) {
		Vec3f pos = { _x, _y, _z };
		Vec3f color = { _r, _g, _b };
		if (!_r && !_g && !_b) color = vec3f_255;

		Light* lt;

		if (type == 'd') {
			if (!pos[x] && !pos[y] && !pos[z]) pos = { 0.0f, 0.0f, -1.0f };
			lt = new Light_Dir(color, pos);
			lights.push_back(lt);
		}

		else if (type == 'p') {
			lt = new Light_Pt(color, pos, falloff);
			lights.push_back(lt);
		}
	}


	void RenderBL::setAmbientLightColor(float _r, float _g, float _b) {
		light_ambient = { _r, _g, _b };
		c_clamp(light_ambient);
	}


	void RenderBL::removeEntity(unsigned int index) {
		if (entities.size()) {
			index %= entities.size();
			entities.erase(entities.begin() + index);
		}
	}


	void RenderBL::removeLight(unsigned int index) {
		if (lights.size()) {
			index %= lights.size();
			delete lights[index];
			lights.erase(lights.begin() + index);
		}
	}


	int RenderBL::coordsToIndex(const Vec2& in) {
		return size[x] * in[y] + in[x];
	}

}





/*
if (((unsigned short)GetKeyState(0x49)) >> 15) { lights[0]->move({ 0, 0, dtime *  2 }); }  // z+ i
if (((unsigned short)GetKeyState(0x4A)) >> 15) { lights[0]->move({ dtime * -2, 0, 0 }); }  // x- j
if (((unsigned short)GetKeyState(0x4B)) >> 15) { lights[0]->move({ 0, 0, dtime * -2 }); }  // z- k
if (((unsigned short)GetKeyState(0x4C)) >> 15) { lights[0]->move({ dtime *  2, 0, 0 }); }  // x+ l
if (((unsigned short)GetKeyState(0x55)) >> 15) { lights[0]->move({ 0, dtime *  2, 0 }); }  // y+ u
if (((unsigned short)GetKeyState(0x4F)) >> 15) { lights[0]->move({ 0, dtime * -2, 0 }); }  // y- o
*/
