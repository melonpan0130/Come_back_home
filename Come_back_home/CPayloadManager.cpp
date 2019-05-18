#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CPayload.h"
#include "CPayloadManager.h"


CPayloadManager::CPayloadManager(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9 texture
	, const D3DXVECTOR3 & center
	, float speed
	, const D3DXVECTOR3 & dir
	, const D3DXVECTOR2 & screensize)
{
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		m_Payload[i] = new CPayload(pD3DDevice
			, texture, center, speed, dir, screensize);
		m_Type[i] = -1;
	}
		
}

CPayloadManager::CPayloadManager(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECT3DTEXTURE9 texture1, LPDIRECT3DTEXTURE9 texture2, LPDIRECT3DTEXTURE9 texture3, const D3DXVECTOR3 & center, float speed, const D3DXVECTOR3 & dir, const D3DXVECTOR2 & screensize)
{
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		m_Payload[i] = new CPayload(pD3DDevice
			, texture1, center, speed, dir, screensize);
		m_Type[i] = -1;
	}
		
	this->m_Texture[0] = texture1;
	this->m_Texture[1] = texture2;
	this->m_Texture[2] = texture3;
}

CPayloadManager::~CPayloadManager()
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
		delete m_Payload[i];
}

int CPayloadManager::getEmptyPayload()
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
		if (m_Payload[i]->IsAlive() == false)
			return i;
	return -1;
}

void CPayloadManager::Update(float dt)
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
		m_Payload[i]->Update(dt);
}

void CPayloadManager::Draw()
{
	for (int i = 0; i < PAYLOAD_MAX; i++)
		m_Payload[i]->Draw();
}

void CPayloadManager::OnFire(const D3DXVECTOR3 & pos)
{
	int id = getEmptyPayload();
	if (id != -1)
		m_Payload[id]->OnFire(pos);
}

void CPayloadManager::OnFire(const D3DXVECTOR3 & pos, int type)
{
	int id = getEmptyPayload();
	m_Payload[id]->SetTexture(m_Texture[type]);
	m_Type[id] = type;
	if (id != -1) {
		m_Payload[id]->SetTexture(m_Texture[type]);
		m_Payload[id]->OnFire(pos);
	}
		
}

void CPayloadManager::OnFire(const D3DXVECTOR3 & pos
	, const D3DXVECTOR3 & dir)
{
	int id = getEmptyPayload();
	if (id != -1)
		m_Payload[id]->OnFire(pos, dir);
}

int CPayloadManager::IsCollision(const D3DXVECTOR3 & pos, float radious)
{
	for(int i=0; i<PAYLOAD_MAX; i++)
		if (m_Payload[i]->IsAlive())
		{
			D3DXVECTOR3 dist = m_Payload[i]->GetPos() - pos;
			if (D3DXVec3Length(&dist) < radious)
			{  
				m_Payload[i]->SetAlive(false);
				if (m_Type[i] != -1) return m_Type[i];
				else return -2;
			}
		}
	return -3;
}

const D3DXVECTOR3 & CPayloadManager::getPos(int slot) const
{
	return m_Payload[slot]->GetPos();
}

bool CPayloadManager::getAlive(int slot)
{
	if (m_Payload[slot]->GetAlive())
		return true;
	return false;
}

void CPayloadManager::setAlive(int slot, bool flag)
{
	m_Payload[slot]->SetAlive(flag);
}