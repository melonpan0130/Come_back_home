#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CSprite.h"
#include "CGameObject.h"


CGameObject::CGameObject()
	:m_Spr(NULL)
	, m_Dir(1.f, 0.f, 0.f)
	, m_Speed(0.f)
	, m_Pos(0.f, 0.f, 0.f)
	, m_Center(0.f, 0.f, 0.f)
{
}

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9 pTexture
	, const D3DXVECTOR3 & center
	, const D3DXVECTOR3 & position
	, float speed)
	:m_Spr(NULL)
	, m_Dir(1.f, 0.f, 0.f)
{
	InitGameObject(pD3DDevice, pTexture, center, position, speed);
}


CGameObject::~CGameObject()
{
	Release();
}

void CGameObject::InitGameObject(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9 pTexture
	, const D3DXVECTOR3 & center
	, const D3DXVECTOR3 & position
	, float speed)
{
	Release();

	m_Spr = new CSprite(pD3DDevice, pTexture, center);
	m_Speed = speed;
	m_Pos = position;
	m_Center = center;
	m_Alive = false;
}

void CGameObject::Release()
{
	if (m_Spr != NULL)
	{
		delete m_Spr;
		m_Spr = NULL;
	}
}

void CGameObject::Update(float dt)
{
	m_Pos += m_Dir * m_Speed * dt;
}

void CGameObject::Render()
{
	if (m_Alive) {
		m_Spr->setPosition(m_Pos);
		m_Spr->Draw();
	}
}

void CGameObject::ArrangePosition(float sx, float ex)
{
	if (m_Pos.x < sx)m_Pos.x = sx;
	if (m_Pos.x > ex)m_Pos.x = ex;
}

bool CGameObject::IsTouched(float leftX, float rightX, bool useRight)
{
	if (m_Alive) {
		if (useRight&&m_Pos.x >= rightX)return true;
		else if (!useRight&&m_Pos.x <= leftX)return true;
	}
	return false;
}
