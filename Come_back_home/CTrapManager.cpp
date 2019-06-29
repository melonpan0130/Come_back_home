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
	, const D3DXVECTOR3& center1
	, const D3DXVECTOR3& center2
	, const D3DXVECTOR3& center3
	, float speed
	, const D3DXVECTOR3 dir
	, const D3DXVECTOR2 screensize
	, float MinX)
	:textureNum(3)
{
	for (int i = 0; i < 2; i++) {
		if (i == 0) { // width
			m_Size[i][0] = center1.x * 2;
			m_Size[i][1] = center2.x * 2;
			m_Size[i][2] = center3.x * 2;
			m_Size[i][3] = center3.x * 2;
		}
		else { // height
			m_Size[i][0] = center1.y * 2;
			m_Size[i][1] = center2.y * 2;
			m_Size[i][2] = center3.y * 2;
			m_Size[i][3] = center3.y * 2;
		}
	}

	for (int i = 0; i < PAYLOAD_MAX; i++)
	{
		m_Payload[0][i] = new CPayload(pD3DDevice
			, texture1, D3DXVECTOR3(0.f, m_Size[1][0], 0.f), speed, dir, screensize, MinX);
		m_Payload[1][i] = new CPayload(pD3DDevice
			, texture2, D3DXVECTOR3(0.f, m_Size[1][1], 0.f), speed, dir, screensize, MinX);
		m_Payload[2][i] = new CPayload(pD3DDevice
			, texture3, D3DXVECTOR3(0.f, m_Size[1][2], 0.f), speed, dir, screensize, MinX);
		m_Payload[3][i] = new CPayload(pD3DDevice
			, texture3, D3DXVECTOR3(0.f, m_Size[1][3], 0.f), speed, dir, screensize, MinX);
	}
}

CTrapManager::CTrapManager(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9 texture1
	, LPDIRECT3DTEXTURE9 texture2
	, LPDIRECT3DTEXTURE9 texture3
	, LPDIRECT3DTEXTURE9 texture4
	, LPDIRECT3DTEXTURE9 texture5
	, const D3DXVECTOR3& center1
	, const D3DXVECTOR3& center2
	, const D3DXVECTOR3& center3
	, const D3DXVECTOR3& center4
	, const D3DXVECTOR3& center5
	, float speed
	, const D3DXVECTOR3 dir
	, const D3DXVECTOR2 screensize
	, float MinX)
	:textureNum(5)
{
	for (int i = 0; i < 2; i++) {
		if (i == 0) { // width
			m_Size[i][0] = center1.x * 2;
			m_Size[i][1] = center2.x * 2;
			m_Size[i][2] = center3.x * 2;
			m_Size[i][3] = center4.x * 2;
			m_Size[i][4] = center5.x * 2;
		}
		else { // height
			m_Size[i][0] = center1.y * 2;
			m_Size[i][1] = center2.y * 2;
			m_Size[i][2] = center3.y * 2;
			m_Size[i][3] = center4.y * 2;
			m_Size[i][4] = center5.y * 2;
		}
	}

	for (int i = 0; i < PAYLOAD_MAX; i++)
	{
		m_Payload[0][i] = new CPayload(pD3DDevice
			, texture1, D3DXVECTOR3(0.f, m_Size[1][0], 0.f), speed, dir, screensize, MinX);
		m_Payload[1][i] = new CPayload(pD3DDevice
			, texture2, D3DXVECTOR3(0.f, m_Size[1][1], 0.f), speed, dir, screensize, MinX);
		m_Payload[2][i] = new CPayload(pD3DDevice
			, texture3, D3DXVECTOR3(0.f, m_Size[1][2], 0.f), speed, dir, screensize, MinX);
		m_Payload[3][i] = new CPayload(pD3DDevice
			, texture4, D3DXVECTOR3(0.f, m_Size[1][3], 0.f), speed, dir, screensize, MinX);
		m_Payload[4][i] = new CPayload(pD3DDevice
			, texture5, D3DXVECTOR3(0.f, m_Size[1][4], 0.f), speed, dir, screensize, MinX);
	}
}

CTrapManager::~CTrapManager()
{
	for (int i = 0; i < textureNum; i++)
		for (int j = 0; j < PAYLOAD_MAX; j++)
			delete m_Payload[i][j];
}

int CTrapManager::getEmptyPayload(int type)
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
		if (!m_Payload[type][i]->IsAlive())
			return i;
	return -1;
}

void CTrapManager::Update(float dt)
{
	for (int i = 0; i < textureNum; i++)
		for (int j = 0; j < PAYLOAD_MAX; j++)
			if (m_Payload[i][j]->IsAlive())
				m_Payload[i][j]->Update(dt);
}

void CTrapManager::Draw()
{
	for (int i = 0; i < textureNum; i++)
		for (int j = 0; j < PAYLOAD_MAX; j++)
			if (m_Payload[i][j]->IsAlive())
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
	for(int i=0; i<textureNum; i++)
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

bool CTrapManager::IsCollision(const D3DXVECTOR3& pos, float radious, int type)
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
		if (m_Payload[type][i]->IsAlive())
		{
			if ((pos.x >= m_Payload[type][i]->GetPos().x - radious
				&& pos.x <= m_Payload[type][i]->GetPos().x + m_Size[0][type] + radious)
				&& (pos.y >= m_Payload[type][i]->GetPos().y - m_Size[1][type] - radious
				&& pos.y <= m_Payload[type][i]->GetPos().y + radious))
			{
				m_Payload[type][i]->SetAlive(false);
				return true;
			}
		}
	return false;
}

void CTrapManager::setSpeedUp(float speedUp, bool flag)
{	
	for (int i = 0; i < textureNum; i++)
			for (int j = 0; j < PAYLOAD_MAX; j++)
				m_Payload[i][j]->setSpeedUp(speedUp, flag);
}

void CTrapManager::setDeadAll()
{
	for (int i = 0; i < textureNum; i++)
		for (int j = 0; j < PAYLOAD_MAX; j++)
			m_Payload[i][j]->SetAlive(false);
}

