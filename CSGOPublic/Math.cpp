#include "stdafx.h"

CMath cMath;

FLOAT fScreenX, fScreenY;

BOOL CMath::ScreenTransform(CONST D3DXVECTOR3& point, OUT D3DXVECTOR3& screen)
{
	const D3DXMATRIX& worldToScreen = pEngine->WorldToScreenMatrix();
	screen.x = worldToScreen.m[0][0] * point.x + worldToScreen.m[0][1] * point.y + worldToScreen.m[0][2] * point.z + worldToScreen.m[0][3];
	screen.y = worldToScreen.m[1][0] * point.x + worldToScreen.m[1][1] * point.y + worldToScreen.m[1][2] * point.z + worldToScreen.m[1][3];

	FLOAT w;
	w = worldToScreen.m[3][0] * point.x + worldToScreen.m[3][1] * point.y + worldToScreen.m[3][2] * point.z + worldToScreen.m[3][3];
	screen.z = 0.0f;

	BOOL behind = FALSE;
	if (w < 0.1f)
	{
		behind = TRUE;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		FLOAT invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}

	return behind;
}

BOOL CMath::WorldToScreen(CONST D3DXVECTOR3 &origin, OUT D3DXVECTOR3 &screen)
{
	BOOL bRet = FALSE;

	if (!ScreenTransform(origin, screen))
	{
		INT width, height;
		pEngine->GetScreenSize(width, height);

		fScreenX = width / 2;
		fScreenY = height / 2;

		fScreenX += 0.5f * screen.x * width + 0.5f;
		fScreenY -= 0.5f * screen.y * height + 0.5f;

		screen.x = fScreenX;
		screen.y = fScreenY;

		bRet = TRUE;
	}

	return bRet;
}
