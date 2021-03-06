#pragma once

#include "vec.h"
#include "texture.h"

namespace ramiel {

	class Draw {
	public:
		Vec3f surfaceColor;

		// triangle vertex data
		Vec3f v_pos[3];
		Vec3f tricam[3];
		Vec2 triscreen[3];

		// lighting
		unsigned specularExponent;
		float specularIntensity;

		// x y clipping
		int x, xmax;
		int y, ymax;

		// change per y
		float dx1_y, dx2_y, *dx_y;
		float dz1_y, dz2_y, *dz_y;

		// scanline start/end
		float x1, x2;
		float z1, z2;

		// change per x
		float dz_x;

		// pixel values
		float z;

		// current pixel/depth buffer array index
		int index;

		void clip1(float c1, float c2, Draw& other);
		void clip2(float c1, float c2);

		void init();
		void swapv(size_t i1, size_t i2);
		void calcd_y();
		void swapdy();
		void clipy();
		void calcd_x();
		void clipx();
		void drawpixel();
		void incx();
		void incy();
		void segmentswitch();
	};


	class DrawFlat : public Draw {
		Vec3f color;

	public:
		void clip1(float c1, float c2, DrawFlat& other);
		void clip2(float c1, float c2);

		void init();
		void swapv(size_t i1, size_t i2);
		void calcd_y();
		void swapdy();
		void clipy();
		void calcd_x();
		void clipx();
		void drawpixel();
		void incx();
		void incy();
		void segmentswitch();
	};


	class DrawVertex : public Draw {
	public:
		Vec3f v_color[3];

	private:
		Vec3f dc1_y, dc2_y, *dc_y;
		Vec3f c1, c2;
		Vec3f dc_x;
		Vec3f c;

	public:
		void clip1(float c1, float c2, DrawVertex& other);
		void clip2(float c1, float c2);

		void init();
		void swapv(size_t i1, size_t i2);
		void calcd_y();
		void swapdy();
		void clipy();
		void calcd_x();
		void clipx();
		void drawpixel();
		void incx();
		void incy();
		void segmentswitch();
	};


	template<class DrawSuper>
	class DrawPS : public DrawSuper {
	public:
		using DrawSuper::surfaceColor;
		using DrawSuper::v_pos;

		using DrawSuper::tricam;
		using DrawSuper::triscreen;

		using DrawSuper::x;
		using DrawSuper::y;
		using DrawSuper::x1;
		using DrawSuper::x2;

	protected:
		float trizinv[3];
		float dzinv1_y, dzinv2_y, * dzinv_y;
		float zinv1, zinv2;
		float dzinv_x;
		float zinv;

	public:
		void clip1(float c1, float c2, DrawPS<DrawSuper>& other) { DrawSuper::clip1(c1, c2, other); }
		void clip2(float c1, float c2) { DrawSuper::clip2(c1, c2); }

		void init() {
			DrawSuper::init();
			for (int i = 0; i < 3; i++) trizinv[i] = 1.0f / tricam[i][Z];
		}

		void swapv(size_t i1, size_t i2) {
			DrawSuper::swapv(i1, i2);
			std::swap(trizinv[i1], trizinv[i2]);
		}

		void calcd_y() {
			DrawSuper::calcd_y();

			// change in inverse z per change in y
			dzinv1_y = (trizinv[2] - trizinv[0]) / (float)(triscreen[2][Y] - triscreen[0][Y]);
			dzinv2_y = (trizinv[1] - trizinv[0]) / (float)(triscreen[1][Y] - triscreen[0][Y]);
			dzinv_y = &dzinv2_y;
		}

		void swapdy() {
			DrawSuper::swapdy();

			std::swap(dzinv1_y, dzinv2_y);
			dzinv_y = &dzinv1_y;
		}

		void clipy() {
			DrawSuper::clipy();

			// start and end inverse z values of scanline
			zinv1 = trizinv[0] + dzinv1_y * (float)(y - triscreen[0][Y]);
			zinv2 = trizinv[0] + dzinv2_y * (float)(y - triscreen[0][Y]);
		}

		void calcd_x() {
			DrawSuper::calcd_x();
			dzinv_x = (zinv2 - zinv1) / (x2 - x1);
		}

		void clipx() {
			DrawSuper::clipx();
			zinv = zinv1 + dzinv_x * (x - (int)x1);
		}

		void drawpixel() {
			DrawSuper::drawpixel();
		}

		void incx() {
			DrawSuper::incx();
			zinv += dzinv_x;
		}

		void incy() {
			DrawSuper::incy();

			zinv1 += dzinv1_y;
			zinv2 += dzinv2_y;
		}

		void segmentswitch() {
			DrawSuper::segmentswitch();

			*dzinv_y = (trizinv[2] - trizinv[1]) / (float)(triscreen[2][Y] - triscreen[1][Y]);
			zinv1 = trizinv[2] - dzinv1_y * (float)(triscreen[2][Y] - y);
			zinv2 = trizinv[2] - dzinv2_y * (float)(triscreen[2][Y] - y);
		}
	};


	class DrawPixel : public DrawPS<Draw> {
		using DrawSuper_ = DrawPS<Draw>;

		Vec3f dp1_y, dp2_y, *dp_y;
		Vec3f p1, p2;
		Vec3f dp_x;
		Vec3f p;

	protected:
		Vec3f normal;

	public:
		void clip1(float c1, float c2, DrawPixel& other);
		void clip2(float c1, float c2);

		void init();
		void swapv(size_t i1, size_t i2);
		void calcd_y();
		void swapdy();
		void clipy();
		void calcd_x();
		void clipx();
		void drawpixel();
		void incx();
		void incy();
		void segmentswitch();
	};


	class DrawPixel_S : public DrawPixel {
	public:
		Vec3f v_normal[3];

	private:
		Vec3f dn1_y, dn2_y, * dn_y;
		Vec3f n1, n2;
		Vec3f dn_x;
		Vec3f n;

	public:
		void clip1(float c1, float c2, DrawPixel_S& other);
		void clip2(float c1, float c2);

		void init();
		void swapv(size_t i1, size_t i2);
		void calcd_y();
		void swapdy();
		void clipy();
		void calcd_x();
		void clipx();
		void drawpixel();
		void incx();
		void incy();
		void segmentswitch();
	};


	template<class DrawSuper>
	class DrawUV : public DrawSuper {
	public:
		using DrawSuper::surfaceColor;

		using DrawSuper::tricam;
		using DrawSuper::triscreen;
		using DrawSuper::trizinv;

		using DrawSuper::x;
		using DrawSuper::y;
		using DrawSuper::x1;
		using DrawSuper::x2;

		using DrawSuper::zinv;

		Vec2f v_txt[3];
		Vec2f du1_y, du2_y, * du_y;
		Vec2f u1, u2;
		Vec2f du_x;
		Vec2f u;

	public:
		void clip1(float c1, float c2, DrawUV<DrawSuper>& other) {
			DrawSuper::clip1(c1, c2, other);

			other.v_txt[0] = v_txt[1] + (v_txt[0] - v_txt[1]) * c1;
			other.v_txt[1] = v_txt[1] + (v_txt[2] - v_txt[1]) * c2;
			other.v_txt[2] = v_txt[2];

			v_txt[1] = other.v_txt[0];
		}

		void clip2(float c1, float c2) {
			DrawSuper::clip2(c1, c2);

			v_txt[0] = v_txt[0] + (v_txt[1] - v_txt[0]) * c1;
			v_txt[2] = v_txt[2] + (v_txt[1] - v_txt[2]) * c2;
		}

		void init() {
			DrawSuper::init();
			for (int i = 0; i < 3; i++) {
				v_txt[i] *= trizinv[i];
			}
		}

		void swapv(size_t i1, size_t i2) {
			DrawSuper::swapv(i1, i2);
			std::swap(v_txt[i1], v_txt[i2]);
		}

		void calcd_y() {
			DrawSuper::calcd_y();

			du1_y = (v_txt[2] - v_txt[0]) / (float)(triscreen[2][Y] - triscreen[0][Y]);
			du2_y = (v_txt[1] - v_txt[0]) / (float)(triscreen[1][Y] - triscreen[0][Y]);
			du_y = &du2_y;
		}

		void swapdy() {
			DrawSuper::swapdy();

			std::swap(du1_y, du2_y);
			du_y = &du1_y;
		}

		void clipy() {
			DrawSuper::clipy();

			u1 = v_txt[0] + du1_y * (float)(y - triscreen[0][Y]);
			u2 = v_txt[0] + du2_y * (float)(y - triscreen[0][Y]);
		}

		void calcd_x() {
			DrawSuper::calcd_x();
			du_x = (u2 - u1) / (x2 - x1);
		}

		void clipx() {
			DrawSuper::clipx();
			u = u1 + du_x * (x - (int)x1);
		}

		void drawpixel() {
			DrawSuper::drawpixel();
		}

		void incx() {
			DrawSuper::incx();
			u += du_x;
		}

		void incy() {
			DrawSuper::incy();

			u1 += du1_y;
			u2 += du2_y;
		}

		void segmentswitch() {
			DrawSuper::segmentswitch();

			*du_y = (v_txt[2] - v_txt[1]) / (float)(triscreen[2][Y] - triscreen[1][Y]);
			u1 = v_txt[2] - du1_y * (float)(triscreen[2][Y] - y);
			u2 = v_txt[2] - du2_y * (float)(triscreen[2][Y] - y);
		}
	};


	template<class DrawSuper>
	class DrawTexture : public DrawUV<DrawSuper> {
		using DrawSuper_ = DrawUV<DrawSuper>;
	public:
		using DrawSuper_::surfaceColor;
		using DrawSuper_::u;
		using DrawSuper_::zinv;
		
		Texture* texture;

		void drawpixel() {
			surfaceColor = texture->get(u / zinv);
			DrawSuper_::drawpixel();
		}
	};

	typedef DrawTexture<DrawPS<DrawFlat>>   DrawFlat_Textured;
	typedef DrawTexture<DrawPS<DrawVertex>> DrawVertex_Textured;
	typedef DrawTexture<DrawPixel>          DrawPixel_Textured;
	typedef DrawTexture<DrawPixel_S>        DrawPixel_S_Textured;


	template<class DrawSuper>
	class DrawNormalmap : public DrawSuper {
	public:
		using DrawSuper::v_pos;
		using DrawSuper::v_txt;
		using DrawSuper::normal;
		using DrawSuper::u;
		using DrawSuper::zinv;

		Texture* normalMap;
	private:
		Vec3f tri_normal;
		Vec3f tri_tangent;
		Vec3f tri_bitangent;

	public:
		void init() {
			const Vec3f dpos1 = v_pos[1] - v_pos[0];
			const Vec3f dpos2 = v_pos[2] - v_pos[0];
			const Vec2f dtex1 = v_txt[1] - v_txt[0];
			const Vec2f dtex2 = v_txt[2] - v_txt[0];

			const float f = 1.0f / (dtex1[X] * dtex2[Y] + dtex2[X] * dtex1[Y]);
			tri_normal    = crossProduct(dpos1, dpos2);
			tri_tangent   = (dpos1 * dtex2[Y] - dpos2 * dtex1[Y]) * f;
			tri_bitangent = (dpos2 * dtex1[X] - dpos1 * dtex2[X]) * f;

			tri_normal = getNormalized(tri_normal);
			tri_tangent = getNormalized(tri_tangent);
			tri_bitangent = getNormalized(tri_bitangent);

			DrawSuper::init();
		}

		void drawpixel() {
			Vec3f normal_in = normalMap->get(u / zinv);
			normal = tri_tangent * normal_in[X] + tri_bitangent * normal_in[Y] + tri_normal * normal_in[Z];
			DrawSuper::drawpixel();
		}
	};

	typedef DrawNormalmap<DrawUV<DrawPixel>>  DrawPixel_NormalMapped;
	typedef DrawNormalmap<DrawPixel_Textured> DrawPixel_Textured_NormalMapped;

}
