#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CPayload.h"
#include "CTrapManager.h"

CTrapManager::CTrapManager(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9 texture1
	, LPDIRECT3DTEXTURE9 texture2
	, LPDIRECT3DTEXTURE9 texture3
	, const D3DXVECTOR3 & center
	, float speed
	, const D3DXVECTOR3 dir
	, const D3DXVECTOR2 screensize)
{
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		m_Payload[0][i] = new CPayload(pD3DDevice
			, texture1, center, speed, dir, screensize);
		m_Payload[1][i] = new CPayload(pD3DDevice
			, texture2, center, speed, dir, screensize);
		m_Payload[2][i] = new CPayload(pD3DDevice
			, texture3, center, speed, dir, screensize);
	}
	
}

CTrapManager::CTrapManager(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9 texture1
	, LPDIRECT3DTEXTURE9 texture2
	, LPDIRECT3DTEXTURE9 texture3
	, const D3DXVECTOR3 & center1
	, const D3DXVECTOR3 & center2
	, const D3DXVECTOR3 & center3
	, float speed
	, const D3DXVECTOR3 dir
	, const D3DXVECTOR2 screensize)
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
	{
		m_Payload[0][i] = new CPayload(pD3DDevice
			, texture1, center1, speed, dir, screensize);
		m_Payload[1][i] = new CPayload(pD3DDevice
			, texture2, center2, speed, dir, screensize);
		m_Payload[2][i] = new CPayload(pD3DDevice
			, texture3, center3, speed, dir, screensize);
	}
}

CTrapManager::~CTrapManager()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < PAYLOAD_MAX; j++)
			delete m_Payload[i][j];
}

int CTrapManager::getEmptyPayload(int type)
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
		if (!m_Payload[type][i]->IsAlive)
			return i;
	return -1;
}

void CTrapManager::Update(float dt)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < PAYLOAD_MAX; j++)
			if (m_Payload[i][j]->IsAlive)
				m_Payload[i][j]->Update(dt);
}

void CTrapManager::Draw()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < PAYLOAD_MAX; j++)
			if (m_Payload[i][j]->IsAlive)
				m_Payload[i][j]->Draw();
}

void CTrapManager::OnFire(const D3DXVECTOR3 & pos, int type)
{
	int id = getEmptyPayload(type);
	if (id != -1)
		m_Payload[type][id]->OnFire(pos);
}

bool CTrapManager::IsCollision(const D3DXVECTOR3 & pos, float radious)
{
	for(int i=0; i<3; i++)
		for(int j=0; j<PAYLOAD_MAX; j++)
			if (m_Payload[i][j]->IsAlive())
			{
				D3DXVECTOR3 dist = m_Payload[i][j]->GetPos() - pos;
				if (D3DXVec3Length(&dist) < radious)
				{
					m_Payload[i][j]->SetAlive(false);
					return true;
				}
			}
	return false;
}
