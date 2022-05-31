#include "draw.h"
#include "ramiel_p.h"

namespace ramiel { 

	void Draw::clip1(float c1, float c2, Draw& other) {
		// interpolate x and y
		other.tricam[0][X] = tricam[1][X] + (tricam[0][X] - tricam[1][X]) * c1;
		other.tricam[0][Y] = tricam[1][Y] + (tricam[0][Y] - tricam[1][Y]) * c1;
		other.tricam[0][Z] = graphics::camera.znear;
		other.tricam[1][X] = tricam[1][X] + (tricam[2][X] - tricam[1][X]) * c2;
		other.tricam[1][Y] = tricam[1][Y] + (tricam[2][Y] - tricam[1][Y]) * c2;
		other.tricam[1][Z] = graphics::camera.znear;
		other.tricam[2] = tricam[2];

		tricam[1] = other.tricam[0];
	}

	void DrawFlat::clip1(float c1, float c2, DrawFlat& other) {
		Draw::clip1(c1, c2, other);
	}

	void DrawVertex::clip1(float c1, float c2, DrawVertex& other) {
		Draw::clip1(c1, c2, other);

		// interpolate color
		other.v_color[0] = v_color[1] + (v_color[0] - v_color[1]) * c1;
		other.v_color[1] = v_color[1] + (v_color[2] - v_color[1]) * c2;
		other.v_color[2] = v_color[2];

		v_color[1] = other.v_color[0];
	}

	void DrawPixel::clip1(float c1, float c2, DrawPixel& other) {
		DrawSuper_::clip1(c1, c2, other);

		// interpolate pos
		other.v_pos[0] = v_pos[1] + (v_pos[0] - v_pos[1]) * c1;
		other.v_pos[1] = v_pos[1] + (v_pos[2] - v_pos[1]) * c2;
		other.v_pos[2] = v_pos[2];

		v_pos[1] = other.v_pos[0];
	}

	void DrawPixel_S::clip1(float c1, float c2, DrawPixel_S& other) {
		DrawPixel::clip1(c1, c2, other);

		// interpolate normal
		other.v_normal[0] = v_normal[1] + (v_normal[0] - v_normal[1]) * c1;
		other.v_normal[1] = v_normal[1] + (v_normal[2] - v_normal[1]) * c2;
		other.v_normal[2] = v_normal[2];

		v_normal[1] = other.v_normal[0];
	}


	void Draw::clip2(float c1, float c2) {
		// interpolate x and y
		tricam[0][X] = tricam[0][X] + (tricam[1][X] - tricam[0][X]) * c1;
		tricam[0][Y] = tricam[0][Y] + (tricam[1][Y] - tricam[0][Y]) * c1;
		tricam[0][Z] = graphics::camera.znear;
		tricam[2][X] = tricam[2][X] + (tricam[1][X] - tricam[2][X]) * c2;
		tricam[2][Y] = tricam[2][Y] + (tricam[1][Y] - tricam[2][Y]) * c2;
		tricam[2][Z] = graphics::camera.znear;
	}

	void DrawFlat::clip2(float c1, float c2) {
		Draw::clip2(c1, c2);
	}

	void DrawVertex::clip2(float c1, float c2) {
		Draw::clip2(c1, c2);

		// interpolate color
		v_color[0] = v_color[0] + (v_color[1] - v_color[0]) * c1;
		v_color[2] = v_color[2] + (v_color[1] - v_color[2]) * c2;
	}

	void DrawPixel::clip2(float c1, float c2) {
		DrawSuper_::clip2(c1, c2);

		// interpolate pos
		v_pos[0] = v_pos[0] + (v_pos[1] - v_pos[0]) * c1;
		v_pos[2] = v_pos[2] + (v_pos[1] - v_pos[2]) * c2;
	}

	void DrawPixel_S::clip2(float c1, float c2) {
		DrawPixel::clip2(c1, c2);

		// interpolate normal
		v_normal[0] = v_normal[0] + (v_normal[1] - v_normal[0]) * c1;
		v_normal[2] = v_normal[2] + (v_normal[1] - v_normal[2]) * c2;
	}


	void Draw::init() {
		// project tri onto screen
		for (int a = 0; a < 3; a++) {
			triscreen[a] = graphics::camera.getScreenCoord(tricam[a]);
		}
	}

	void DrawFlat::init() {
		Draw::init();

		Vec3f pos = (v_pos[0] + v_pos[1] + v_pos[2]) / 3.0f;

		Vec3f v1 = v_pos[1] - v_pos[0];
		Vec3f v2 = v_pos[2] - v_pos[0];
		Vec3f normal = getNormalized(crossProduct(v1, v2));

		color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
	}

	void DrawVertex::init() {
		Draw::init();
	}

	void DrawPixel::init() {
		DrawSuper_::init();

		Vec3f v1 = v_pos[1] - v_pos[0];
		Vec3f v2 = v_pos[2] - v_pos[0];
		normal = getNormalized(crossProduct(v1, v2));
		for (int i = 0; i < 3; i++) v_pos[i] *= trizinv[i];
	}

	void DrawPixel_S::init() {
		DrawPixel::init();
	}

	void Draw::swapv(size_t i1, size_t i2) {
		std::swap(triscreen[i1], triscreen[i2]);
		std::swap(tricam[i1], tricam[i2]);
	}

	void DrawFlat::swapv(size_t i1, size_t i2) {
		Draw::swapv(i1, i2);
	}

	void DrawVertex::swapv(size_t i1, size_t i2) {
		Draw::swapv(i1, i2);
		std::swap(v_color[i1], v_color[i2]);
	}

	void DrawPixel::swapv(size_t i1, size_t i2) {
		DrawSuper_::swapv(i1, i2);
		std::swap(v_pos[i1], v_pos[i2]);
	}

	void DrawPixel_S::swapv(size_t i1, size_t i2) {
		DrawPixel::swapv(i1, i2);
		std::swap(v_normal[i1], v_normal[i2]);
	}

	void Draw::calcd_y() {
		// change in x per change in y
		dx1_y = (float)(triscreen[2][X] - triscreen[0][X]) / (float)(triscreen[2][Y] - triscreen[0][Y]);
		dx2_y = (float)(triscreen[1][X] - triscreen[0][X]) / (float)(triscreen[1][Y] - triscreen[0][Y]);
		dx_y = &dx2_y;

		// change in z per change in y
		dz1_y = (tricam[2][Z] - tricam[0][Z]) / (float)(triscreen[2][Y] - triscreen[0][Y]);
		dz2_y = (tricam[1][Z] - tricam[0][Z]) / (float)(triscreen[1][Y] - triscreen[0][Y]);
		dz_y = &dz2_y;
	}

	void DrawFlat::calcd_y() {
		Draw::calcd_y();
	}

	void DrawVertex::calcd_y() {
		Draw::calcd_y();

		// change in color per change in y
		dc1_y = (v_color[2] - v_color[0]) / (float)(triscreen[2][Y] - triscreen[0][Y]);
		dc2_y = (v_color[1] - v_color[0]) / (float)(triscreen[1][Y] - triscreen[0][Y]);
		dc_y = &dc2_y;
	}

	void DrawPixel::calcd_y() {
		DrawSuper_::calcd_y();

		// change in pos per change in y
		dp1_y = (v_pos[2] - v_pos[0]) / (float)(triscreen[2][Y] - triscreen[0][Y]);
		dp2_y = (v_pos[1] - v_pos[0]) / (float)(triscreen[1][Y] - triscreen[0][Y]);
		dp_y = &dp2_y;
	}

	void DrawPixel_S::calcd_y() {
		DrawPixel::calcd_y();

		// change in normal per change in y
		dn1_y = (v_normal[2] - v_normal[0]) / (float)(triscreen[2][Y] - triscreen[0][Y]);
		dn2_y = (v_normal[1] - v_normal[0]) / (float)(triscreen[1][Y] - triscreen[0][Y]);
		dn_y = &dn2_y;
	}

	void Draw::swapdy() {
		std::swap(dx1_y, dx2_y);
		std::swap(dz1_y, dz2_y);
		dx_y = &dx1_y;
		dz_y = &dz1_y;
	}

	void DrawFlat::swapdy() {
		Draw::swapdy();
	}

	void DrawVertex::swapdy() {
		Draw::swapdy();

		std::swap(dc1_y, dc2_y);
		dc_y = &dc1_y;
	}

	void DrawPixel::swapdy() {
		DrawSuper_::swapdy();

		std::swap(dp1_y, dp2_y);
		dp_y = &dp1_y;
	}

	void DrawPixel_S::swapdy() {
		DrawPixel::swapdy();

		std::swap(dn1_y, dn2_y);
		dn_y = &dn1_y;
	}

	void Draw::clipy() {
		// y value of scanline
		y = std::max(0, triscreen[0][Y]);
		ymax = std::min<int>(triscreen[1][Y], graphics::size[Y]);

		// start and end x values of scanline
		x1 = (float)triscreen[0][X] + dx1_y * (float)(y - triscreen[0][Y]);
		x2 = (float)triscreen[0][X] + dx2_y * (float)(y - triscreen[0][Y]);

		// start and end z values of scanline
		z1 = tricam[0][Z] + dz1_y * (float)(y - triscreen[0][Y]);
		z2 = tricam[0][Z] + dz2_y * (float)(y - triscreen[0][Y]);
	}

	void DrawFlat::clipy() {
		Draw::clipy();
	}

	void DrawVertex::clipy() {
		Draw::clipy();

		// start and end color values of scanline
		c1 = v_color[0] + dc1_y * (float)(y - triscreen[0][Y]);
		c2 = v_color[0] + dc2_y * (float)(y - triscreen[0][Y]);
	}

	void DrawPixel::clipy() {
		DrawSuper_::clipy();

		// start and end pos values of scanline
		p1 = v_pos[0] + dp1_y * (float)(y - triscreen[0][Y]);
		p2 = v_pos[0] + dp2_y * (float)(y - triscreen[0][Y]);
	}

	void DrawPixel_S::clipy() {
		DrawPixel::clipy();

		// start and end normal values of scanline
		n1 = v_normal[0] + dn1_y * (float)(y - triscreen[0][Y]);
		n2 = v_normal[0] + dn2_y * (float)(y - triscreen[0][Y]);
	}

	void Draw::calcd_x() {
		dz_x = (z2 - z1) / (x2 - x1);
	}

	void DrawFlat::calcd_x() {
		Draw::calcd_x();
	}

	void DrawVertex::calcd_x() {
		Draw::calcd_x();
		dc_x = (c2 - c1) / (x2 - x1);
	}

	void DrawPixel::calcd_x() {
		DrawSuper_::calcd_x();
		dp_x = (p2 - p1) / (x2 - x1);
	}

	void DrawPixel_S::calcd_x() {
		DrawPixel::calcd_x();
		dn_x = (n2 - n1) / (x2 - x1);
	}

	void Draw::clipx() {
		xmax = std::min<int>(x2, graphics::size[X]);
		x = std::max(0, (int)x1);
		z = z1 + dz_x * (x - (int)x1);
		index = graphics::coordsToIndex({ x, y });
	}

	void DrawFlat::clipx() {
		Draw::clipx();
	}

	void DrawVertex::clipx() {
		Draw::clipx();
		c = c1 + dc_x * (x - x1);
	}

	void DrawPixel::clipx() {
		DrawSuper_::clipx();
		p = p1 + dp_x * (x - (int)x1);
	}

	void DrawPixel_S::clipx() {
		DrawPixel::clipx();
		n = n1 + dn_x * (x - (int)x1);
	}

	void Draw::drawpixel() {
		throw std::string("cannot draw without shading type");
	}

	void DrawFlat::drawpixel() {
		graphics::depth[index] = z;
		graphics::pixels[index] = color * surfaceColor;
	}

	void DrawVertex::drawpixel() {
		graphics::depth[index] = z;
		graphics::pixels[index] = c * surfaceColor;
	}

	void DrawPixel::drawpixel() {
		graphics::depth[index] = z;
		Vec3f pos = p / zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[index] = color * surfaceColor;
	}

	void DrawPixel_S::drawpixel() {
		normal = getNormalized(n);
		DrawPixel::drawpixel();
	}

	void Draw::incx() {
		index++;
		z += dz_x;
	}

	void DrawFlat::incx() {
		Draw::incx();
	}

	void DrawVertex::incx() {
		Draw::incx();
		c += dc_x;
	}

	void DrawPixel::incx() {
		DrawSuper_::incx();
		p += dp_x;
	}

	void DrawPixel_S::incx() {
		DrawPixel::incx();
		n += dn_x;
	}

	void Draw::incy() {
		x1 += dx1_y;
		x2 += dx2_y;

		z1 += dz1_y;
		z2 += dz2_y;
	}

	void DrawFlat::incy() {
		Draw::incy();
	}

	void DrawVertex::incy() {
		Draw::incy();

		c1 += dc1_y;
		c2 += dc2_y;
	}

	void DrawPixel::incy() {
		DrawSuper_::incy();

		p1 += dp1_y;
		p2 += dp2_y;
	}

	void DrawPixel_S::incy() {
		DrawPixel::incy();

		n1 += dn1_y;
		n2 += dn2_y;
	}

	void Draw::segmentswitch() {
		ymax = std::min<int>(triscreen[2][Y], graphics::size[Y]);

		*dx_y = (float)(triscreen[2][X] - triscreen[1][X]) / (float)(triscreen[2][Y] - triscreen[1][Y]);
		x1 = (float)triscreen[2][X] - dx1_y * (float)(triscreen[2][Y] - y);
		x2 = (float)triscreen[2][X] - dx2_y * (float)(triscreen[2][Y] - y);

		*dz_y = (tricam[2][Z] - tricam[1][Z]) / (float)(triscreen[2][Y] - triscreen[1][Y]);
		z1 = tricam[2][Z] - dz1_y * (float)(triscreen[2][Y] - y);
		z2 = tricam[2][Z] - dz2_y * (float)(triscreen[2][Y] - y);
	}

	void DrawFlat::segmentswitch() {
		Draw::segmentswitch();
	}

	void DrawVertex::segmentswitch() {
		Draw::segmentswitch();

		*dc_y = (v_color[2] - v_color[1]) / (float)(triscreen[2][Y] - triscreen[1][Y]);
		c1 = v_color[2] - dc1_y * (float)(triscreen[2][Y] - y);
		c2 = v_color[2] - dc2_y * (float)(triscreen[2][Y] - y);
	}

	void DrawPixel::segmentswitch() {
		DrawSuper_::segmentswitch();

		*dp_y = (v_pos[2] - v_pos[1]) / (float)(triscreen[2][Y] - triscreen[1][Y]);
		p1 = v_pos[2] - dp1_y * (float)(triscreen[2][Y] - y);
		p2 = v_pos[2] - dp2_y * (float)(triscreen[2][Y] - y);
	}

	void DrawPixel_S::segmentswitch() {
		DrawPixel::segmentswitch();

		*dn_y = (v_normal[2] - v_normal[1]) / (float)(triscreen[2][Y] - triscreen[1][Y]);
		n1 = v_normal[2] - dn1_y * (float)(triscreen[2][Y] - y);
		n2 = v_normal[2] - dn2_y * (float)(triscreen[2][Y] - y);
	}

}
