#pragma once
class CBackground
{
public:
	explicit CBackground(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 pTexture
		, float imgLength
		, float speed
		, boolean vertical = false);
	virtual ~CBackground();

	void Update(float dt);
	void Render();

	void setScroll(float point)
	{
		m_Scroll = point;
		if (m_Scroll < -m_ImgLength)
			m_Scroll += m_ImgLength;
	}

protected:
	CSprite* m_Spr[3];

	D3DXVECTOR3 m_BasePos;
	float m_Scroll;
	float m_ImgLength; // be width or height
	float m_ScrollSpeed;
	boolean IsVertical;
};

