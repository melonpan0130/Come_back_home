#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CSprite.h"
#include "CBackground.h"


CBackground::CBackground(LPDIRECT3DDEVICE9 pD3DDevice
	, LPDIRECT3DTEXTURE9 pTexture
	, float imgLength
	, float speed
	, boolean vertical)
	:m_BasePos(D3DXVECTOR3(0,0,0))
	, m_ImgLength(imgLength)
	,m_ScrollSpeed(speed)
	,m_Scroll(0.1f)
{
	for(int i=0; i<3; i++)
		m_Spr[i] = new CSprite(pD3DDevice, pTexture, D3DXVECTOR3(0.f, 0.f, 0.f));
}

CBackground::~CBackground()
{
	for (int i = 0; i < 3; i++)
	{
		if (m_Spr[i] != NULL)
		{
			delete m_Spr[i];
			m_Spr[i] = NULL;
		}
	}
}

void CBackground::Update(float dt)
{
	m_Scroll -= (m_ScrollSpeed * dt);
	if (m_Scroll < -m_ImgLength)
		m_Scroll += m_ImgLength;

	D3DXVECTOR3 Pos1, Pos2, Pos3;
	if (IsVertical) {
		Pos1 = D3DXVECTOR3(m_BasePos.x, m_Scroll, 0.f);
		Pos2 = Pos1 + D3DXVECTOR3(0.f, m_ImgLength - 1.f, 0.f);
		Pos3 = Pos2 + D3DXVECTOR3(0.f, m_ImgLength - 1.f, 0.f);
	}
	else {
		Pos1 = D3DXVECTOR3(m_Scroll, m_BasePos.y, 0.f);
		Pos2 = Pos1 + D3DXVECTOR3(m_ImgLength - 1.f, 0.f, 0.f);
		Pos3 = Pos2 + D3DXVECTOR3(m_ImgLength - 1.f, 0.f, 0.f);
	}
	
	m_Spr[0]->setPosition(Pos1);
	m_Spr[1]->setPosition(Pos2);
	m_Spr[2]->setPosition(Pos3);
}

void CBackground::Render()
{
	m_Spr[0]->Draw();
	m_Spr[1]->Draw();
	m_Spr[2]->Draw();
}