#include "stdafx.h"

#define MAX_ENTS 64

CDrone *gDrone;

void CDrone::UpdateDraw()
{
	IClientEntity *pLocal = pClientEntList->GetClientEntity(pEngine->GetLocalPlayer());
	int numEnts = pClientEntList->GetHighestEntityIndex();

	for (int i = 0; i < MAX_ENTS; i++)
	{
		IClientEntity *pEntity = pClientEntList->GetClientEntity(i);

		if (!pEntity)
			continue;

		if (pEntity == pLocal)
			continue;

		if (!pEntity->GetHealth())
			continue;

		if (pEntity->GetDormant())
			continue;

		//everything seems okay with this ent

		ESPEngage(i, pEntity);
	}
}
void CDrone::UpdateAim()
{
	IClientEntity *pLocal = pClientEntList->GetClientEntity(pEngine->GetLocalPlayer());
	int numEnts = pClientEntList->GetHighestEntityIndex();

	for (int i = 0; i < MAX_ENTS; i++)
	{
		IClientEntity *pEntity = pClientEntList->GetClientEntity(i);

		if (!pEntity)
			continue;

		if (pEntity == pLocal)
			continue;

		if (!pEntity->GetHealth())
			continue;

		if (pEntity->GetDormant())
			continue;

		//everything seems okay with this ent
		AimEngage(i, pEntity);
	}
	
}
void CDrone::AimEngage(int i, IClientEntity *pEntity)
{
	//todo
}

void CDrone::ESPEngage(int i, IClientEntity *pEntity)
{
	IClientEntity *pLocal = pClientEntList->GetClientEntity(pEngine->GetLocalPlayer());
	player_info_t enemyInfo;
	pEngine->GetPlayerInfo(i, &enemyInfo);

	Color drawColor = Color(255, 255, 255, 255);
	D3DXVECTOR3 bonePos;

	if (!GetBonePosition(pEntity, bonePos, 6))
		return;

	if (pEntity->GetTeamNum() == 3)
		drawColor = Color(0, 0, 200, 255);
	else
		drawColor = Color(200, 0, 0, 255);

	D3DXVECTOR3 headPos, Screen;

	int Health;
	Health = pEntity->GetHealth();

	if (Health > 0)
	{
		if (cMath.WorldToScreen(bonePos, headPos) && cMath.WorldToScreen(pEntity->GetOrigin(), Screen))
		{
			float Height = (Screen.y - headPos.y) + 5.0f;
			float Width = Height / 1.75;

			if (Width < 15)
				Width = 15;

			//name
			cDraw.DrawString(Screen.x, Screen.y - Height - 15, drawColor, SegoeUI, true, "%s", enemyInfo.name); //not scaling font height so w/e

			//Health
			pSurface->DrawSetColor(Color(0, 0, 0, 255));
			pSurface->DrawFilledRect((int)Screen.x - (Width / 2) - 0.5, (int)Screen.y, (int)Screen.x + (Width/2) + 0.5, (int)Screen.y + 4.25);
			pSurface->DrawSetColor(drawColor);
			pSurface->DrawFilledRect((int)Screen.x - (Width / 2), (int)Screen.y + 0.5, (int)Screen.x + (Width/2) * (Health/100), (int)Screen.y + 3.75);

			//BoneESP(pEntity);

			//boxes
			pSurface->DrawSetColor(drawColor);
			Screen.x = Screen.x - (Width / 2);
			//bottom left 2
			pSurface->DrawLine(Screen.x, Screen.y, Screen.x + (Width / 4), Screen.y);
			pSurface->DrawLine(Screen.x, Screen.y, Screen.x, Screen.y - (Height / 4));
			//bottom right 2
			pSurface->DrawLine(Screen.x + Width, Screen.y, Screen.x + ((Width / 4) * 3), Screen.y);
			pSurface->DrawLine(Screen.x + Width, Screen.y, Screen.x + Width, Screen.y - (Height / 4));
			//top left 2
			pSurface->DrawLine(Screen.x, Screen.y - Height, Screen.x + (Width / 4), Screen.y - Height);
			pSurface->DrawLine(Screen.x, Screen.y - Height, Screen.x, Screen.y - ((Height / 4) * 3));
			//top right 2
			pSurface->DrawLine(Screen.x + Width, Screen.y - Height, Screen.x + ((Width / 4) * 3), Screen.y - Height);
			pSurface->DrawLine(Screen.x + Width, Screen.y - Height, Screen.x + Width, Screen.y - ((Height / 4) * 3));

		}
	}
}

void CDrone::BoneESP(IClientEntity *pEntity)
{
	D3DXVECTOR3 pBonePos[21];
	D3DXVECTOR3 pBoneScreen[21];
	pSurface->DrawSetColor(Color(255, 255, 255, 255));

	if (!GetBonePosition(pEntity, pBonePos[0], 6) || !cMath.WorldToScreen(pBonePos[0], pBoneScreen[0]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[1], 35) || !cMath.WorldToScreen(pBonePos[1], pBoneScreen[1]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[2], 0) || !cMath.WorldToScreen(pBonePos[2], pBoneScreen[2]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[3], 8) || !cMath.WorldToScreen(pBonePos[3], pBoneScreen[3]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[4], 9) || !cMath.WorldToScreen(pBonePos[4], pBoneScreen[4]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[5], 2) || !cMath.WorldToScreen(pBonePos[5], pBoneScreen[5]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[6], 63) || !cMath.WorldToScreen(pBonePos[6], pBoneScreen[6]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[7], 67) || !cMath.WorldToScreen(pBonePos[7], pBoneScreen[7]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[8], 68) || !cMath.WorldToScreen(pBonePos[8], pBoneScreen[8]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[9], 64) || !cMath.WorldToScreen(pBonePos[9], pBoneScreen[9]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[10], 65) || !cMath.WorldToScreen(pBonePos[10], pBoneScreen[10]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[11], 66) || !cMath.WorldToScreen(pBonePos[11], pBoneScreen[11]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[12], 36) || !cMath.WorldToScreen(pBonePos[12], pBoneScreen[12]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[13], 37) || !cMath.WorldToScreen(pBonePos[13], pBoneScreen[13]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[14], 60) || !cMath.WorldToScreen(pBonePos[14], pBoneScreen[14]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[15], 69) || !cMath.WorldToScreen(pBonePos[15], pBoneScreen[15]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[16], 73) || !cMath.WorldToScreen(pBonePos[16], pBoneScreen[16]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[17], 74) || !cMath.WorldToScreen(pBonePos[17], pBoneScreen[17]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[18], 70) || !cMath.WorldToScreen(pBonePos[18], pBoneScreen[18]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[19], 71) || !cMath.WorldToScreen(pBonePos[19], pBoneScreen[19]))
		return;
	if (!GetBonePosition(pEntity, pBonePos[20], 72) || !cMath.WorldToScreen(pBonePos[20], pBoneScreen[20]))
		return;

	pSurface->DrawLine(pBoneScreen[0].x, pBoneScreen[0].y, pBoneScreen[1].x, pBoneScreen[1].y);
	pSurface->DrawLine(pBoneScreen[1].x, pBoneScreen[1].y, pBoneScreen[2].x, pBoneScreen[2].y);

	//Right Arm
	pSurface->DrawLine(pBoneScreen[1].x, pBoneScreen[1].y, pBoneScreen[3].x, pBoneScreen[3].y);
	pSurface->DrawLine(pBoneScreen[3].x, pBoneScreen[3].y, pBoneScreen[4].x, pBoneScreen[4].y);
	pSurface->DrawLine(pBoneScreen[4].x, pBoneScreen[4].y, pBoneScreen[5].x, pBoneScreen[5].y);

	//Left Arm
	pSurface->DrawLine(pBoneScreen[1].x, pBoneScreen[1].y, pBoneScreen[12].x, pBoneScreen[12].y);
	pSurface->DrawLine(pBoneScreen[12].x, pBoneScreen[12].y, pBoneScreen[13].x, pBoneScreen[13].y);
	pSurface->DrawLine(pBoneScreen[13].x, pBoneScreen[13].y, pBoneScreen[14].x, pBoneScreen[14].y);

	//Right Leg
	pSurface->DrawLine(pBoneScreen[2].x, pBoneScreen[2].y, pBoneScreen[6].x, pBoneScreen[6].y);
	pSurface->DrawLine(pBoneScreen[6].x, pBoneScreen[6].y, pBoneScreen[7].x, pBoneScreen[7].y);
	pSurface->DrawLine(pBoneScreen[7].x, pBoneScreen[7].y, pBoneScreen[8].x, pBoneScreen[8].y);
	pSurface->DrawLine(pBoneScreen[8].x, pBoneScreen[8].y, pBoneScreen[9].x, pBoneScreen[9].y);
	pSurface->DrawLine(pBoneScreen[9].x, pBoneScreen[9].y, pBoneScreen[10].x, pBoneScreen[10].y);
	pSurface->DrawLine(pBoneScreen[10].x, pBoneScreen[11].y, pBoneScreen[10].x, pBoneScreen[10].y);


	//Left Leg
	pSurface->DrawLine(pBoneScreen[2].x, pBoneScreen[2].y, pBoneScreen[15].x, pBoneScreen[15].y);
	pSurface->DrawLine(pBoneScreen[15].x, pBoneScreen[15].y, pBoneScreen[16].x, pBoneScreen[16].y);
	pSurface->DrawLine(pBoneScreen[16].x, pBoneScreen[16].y, pBoneScreen[17].x, pBoneScreen[17].y);
	pSurface->DrawLine(pBoneScreen[17].x, pBoneScreen[17].y, pBoneScreen[18].x, pBoneScreen[18].y);
	pSurface->DrawLine(pBoneScreen[18].x, pBoneScreen[18].y, pBoneScreen[19].x, pBoneScreen[19].y);
	pSurface->DrawLine(pBoneScreen[19].x, pBoneScreen[19].y, pBoneScreen[20].x, pBoneScreen[20].y);


}

bool CDrone::GetBonePosition(IClientEntity* pPlayer, D3DXVECTOR3& Hitbox, int Bone)
{
	matrix3x4_t MatrixArray[128];

	if (!pPlayer->SetupBones(MatrixArray, 128, 0x00000100, pEngine->Time()))
		return FALSE;

	matrix3x4_t HitboxMatrix = MatrixArray[Bone];

	Hitbox = D3DXVECTOR3(HitboxMatrix[0][3], HitboxMatrix[1][3], HitboxMatrix[2][3]);

	return TRUE;
}