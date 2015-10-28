#pragma once

class CDrone
{
public:
	void UpdateDraw();
	void UpdateAim();
	void AimEngage(int i, IClientEntity *pEntity);
	void ESPEngage(int i, IClientEntity *pEntity);
	void BoneESP(IClientEntity *pEntity);
	bool GetBonePosition(IClientEntity* pPlayer, D3DXVECTOR3& Hitbox, int Bone);
};


extern CDrone *gDrone;

class IClientMode
{

}; extern IClientMode ** ClientMode;