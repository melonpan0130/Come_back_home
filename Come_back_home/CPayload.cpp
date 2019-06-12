#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CSprite.h"
#include "CPayload.h"


CPayload::CPayload(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9& texture
	, const D3DXVECTOR3& center
	, float speed
	, const D3DXVECTOR3& dir
	, const D3DXVECTOR2 screensize
	, float MinX)
	:m_Alive(false)
	, m_Speed(speed)
	, m_Dir(dir)
	, m_ScreenSize(screensize)
	, m_MinX(MinX)
{
	m_Spr = new CSprite(pD3DDevice, texture, center);
}

CPayload::~CPayload()
{
	if (m_Spr != NULL)
	{
		m_Spr->Release();
		m_Spr = NULL;
	}
}

void CPayload::SetTexture(const LPDIRECT3DTEXTURE9 texture)
{
	m_Spr->setTexture(texture);
}

void CPayload::Update(float dt)
{
	if (m_Alive)
	{
		m_Pos += m_Dir * dt*m_Speed;

		if (m_Pos.y<m_MinX || m_Pos.y>m_ScreenSize.y ||
			m_Pos.x<m_MinX || m_Pos.x>m_ScreenSize.x)
			m_Alive = false;
	}
}

void CPayload::Draw()
{
	if (m_Alive)
	{
		m_Spr->setPosition(m_Pos);
		m_Spr->Draw();
	}
}
