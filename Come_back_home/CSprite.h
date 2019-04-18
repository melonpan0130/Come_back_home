#pragma once
class CSprite
{
public:
	CSprite(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 pTexture
		, const D3DXVECTOR3 center);
	virtual ~CSprite();

	void InitSprite();
	void Release();

	void setPosition(const D3DXVECTOR3& pos) { m_Pos = pos; }
	void setTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

	void Draw();
protected:
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPD3DXSPRITE m_pSpr;
	LPDIRECT3DTEXTURE9 m_pTexture;

	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Center;
	D3DCOLOR m_Color;
};

