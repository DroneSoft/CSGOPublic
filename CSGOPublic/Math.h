#ifndef	Q_MATH_H
#define	Q_MATH_H

class CMath;

class CMath
{
public:
	BOOL WorldToScreen(CONST D3DXVECTOR3& point, OUT D3DXVECTOR3& screen);
	BOOL ScreenTransform(CONST D3DXVECTOR3& point, OUT D3DXVECTOR3& screen);
};
extern CMath cMath;

#endif