#include "stdafx.h"
#include <stdlib.h>

CDrawing cDraw;

void CDrawing::DrawString(int X, int Y, Color Color, unsigned long Font, bool Center, char* _Input, ...)
{
	/* set up buffer */
	char Buffer[1024] = { '\0' };

	/* set up varargs*/
	va_list Args;

	va_start(Args, _Input);
	vsprintf_s(Buffer, _Input, Args);
	va_end(Args);

	size_t Size = strlen(Buffer) + 1;

	/* set up widebuffer*/
	wchar_t* WideBuffer = new wchar_t[Size];

	/* char -> wchar */
	mbstowcs_s(0, WideBuffer, Size, Buffer, Size - 1);

	/* check center */
	int Width = 0, Height = 0;

	if (Center) {
		pSurface->GetTextSize(Font, WideBuffer, Width, Height);
	}

	/* call and draw*/
	pSurface->DrawSetTextColor(Color);
	pSurface->DrawSetTextFont(Font);
	pSurface->DrawSetTextPos(X - (Width * .5), Y);
	pSurface->DrawPrintText(WideBuffer, wcslen(WideBuffer), FONT_DRAW_DEFAULT);
}
void CDrawing::OutlinedRect(int x, int y, int w, int h, Color& color)
{
	pSurface->DrawSetColor(color);
	pSurface->DrawOutlinedRect(x, y, x - w, y - h);
}

void CDrawing::FillRGBA(int x, int y, int w, int h, Color colour)
{
	pSurface->DrawSetColor(colour);
	pSurface->DrawFilledRect(x, y, x + w, y + h);
}

void CDrawing::OutlineRGBA(int x, int y, int w, int h, int lw, Color color)
{
	FillRGBA(x, y, w, lw, color);
	FillRGBA(x, y, lw, h, color);
	FillRGBA(x, y + h, w + lw, lw, color);
	FillRGBA(x + w, y, lw, h, color);
}

void CDrawing::DrawHealthBar(int x, int y, float health, int w, int h, Color Draw)
{
	x -= w / 2;
	y -= h / 2;
	cDraw.FillRGBA(x, y, w, h + 1, Color(0, 0, 0, 255));
	UINT hw = (UINT)(((w - 2) * health) / 100);
	cDraw.FillRGBA(x + 1, y + 1, hw, h - 1, Draw);
}