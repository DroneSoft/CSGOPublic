#pragma once

class CDrawing
{
public:
	void DrawString(int X, int Y, Color Color, unsigned long Font, bool Center, char* _Input, ...);
	void OutlinedRect(int x, int y, int w, int h, Color& color);
	void DrawHealthBar(int x, int y, float health, int w, int h, Color Draw);
	void FillRGBA(int x, int y, int w, int h, Color colour);
};

extern CDrawing cDraw;