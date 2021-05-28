#include <vector>
#include "global.h"
#include "clip.h"


void connectLines(std::vector<Line2D>& edges) {
	// connect distance between end of line and
	// start of adjacent line by inserting new line
	Vec2 pt1;
	Vec2 pt2;
	for (int a = 0; a < (int)edges.size() - 1; a++) {
		pt1 = edges[a].end;
		pt2 = edges[a + 1].start;
		if (pt1.x != pt2.x || pt1.y != pt2.y) {
			edges.insert(edges.begin() + a + 1, Line2D(pt1, pt2));
		}
	}
	// connect final line in list to first
	pt1 = edges[edges.size() - 1].end;
	pt2 = edges[0].start;
	if (pt1.x != pt2.x || pt1.y != pt2.y) {
		edges.push_back(Line2D(pt1, pt2));
	}
}


void clipTriangle(std::vector<Tri2D>& clippedTris, Tri2D triInput) {
	int xMin = 0;
	int yMin = 0;
	int xMax = winSizeX - 1;
	int yMax = winSizeY - 1;

	// convert input tri to edges
	std::vector<Line2D> edges;
	edges.push_back(Line2D(triInput.pts[0], triInput.pts[1]));
	edges.push_back(Line2D(triInput.pts[1], triInput.pts[2]));
	edges.push_back(Line2D(triInput.pts[2], triInput.pts[0]));

	// clip each line in triangle (x-axis)
	for (int a = 0; a < (int)edges.size(); a++) {
		int lineState = edges[a].clipX(xMin, xMax);
		if (lineState == 5 || lineState == 10) {
			edges.erase(edges.begin() + a);
			a--;
		}
	}

	// return early if tri is not in frame
	if (edges.size() == 0) return;

	// connect gaps in lines with new lines
	connectLines(edges);

	// clip each line in triangle (y-axis)
	for (int a = 0; a < (int)edges.size(); a++) {
		int lineState = edges[a].clipY(yMin, yMax);
		if (lineState == 5 || lineState == 10) {
			edges.erase(edges.begin() + a);
			a--;
		}
	}

	// return early if tri is not in frame
	if (edges.size() == 0) return;

	// connect gaps in lines with new lines
	connectLines(edges);

	// divide polygon into triangles
	for (int a = 0; a < (int)edges.size() - 2; a++) {
		Tri2D newTri;
		newTri.pts[0] = edges[0].end;
		newTri.pts[1] = edges[a + 1].end;
		newTri.pts[2] = edges[a + 2].end;
		clippedTris.push_back(newTri);
	}
}


// coming soon
void clipZaxis(std::vector<Tri3D>& clippedTris, Tri3D triInput) {
	if (triInput.pts[0].z > 0);
}
