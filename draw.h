#ifndef BL_DRAW_H
#define BL_DRAW_H

#include "vec.h"

namespace bl {

	class Draw {
	public:

		// triangle vertex data
		Vertex v[3];
		Vec3f tricam[3];
		Vec2 triscreen[3];

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
		// change per y
		Vec3f dc1_y, dc2_y, *dc_y;

		// scanline start/end
		Vec3f c1, c2;

		// change per x
		Vec3f dc_x;

		// pixel values
		Vec3f c;

	public:
		void clip1(float c1, float c2, DrawVertex& other);
		void clip2(float c1, float c2);

		void init();
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

	class DrawPixel : public Draw {
		// zinv triangle
		float trizinv[3];

		// change per y
		float dzinv1_y, dzinv2_y, *dzinv_y;
		Vec3f dp1_y, dp2_y, *dp_y;
		Vec3f dn1_y, dn2_y, *dn_y;
			
		// scanline start/end
		float zinv1, zinv2;
		Vec3f p1, p2;
		Vec3f n1, n2;

		// change per x
		float dzinv_x;
		Vec3f dp_x;
		Vec3f dn_x;

		// pixel values
		float zinv;
		Vec3f p;
		Vec3f n;

	public:
		void clip1(float c1, float c2, DrawPixel& other);
		void clip2(float c1, float c2);

		void init();
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

}

#endif
