#ifndef DRAW_H_
#define DRAW_H_


void drawBackground();

void drawRect(unsigned int color, int x1, int y1, int len, int ht);

void drawHzLine(int x1, int x2, int y);
void drawVertLine(int y1, int y2, int x);
void drawLine(int x1, int y1, int x2, int y2);

void drawTriangle(struct Tri2D* tri);


#endif
