#pragma once
class CBackground
{
public:
	explicit CBackground(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 pTexture
		, float imgLength
		, float speed);
	virtual ~CBackground();

	void Update(float dt);
	void UpdateVertical(float dt);
	void Render();

	void setScroll(float point)
	{
		m_Scroll = point;
		if (m_Scroll < -m_ImgLength)
			m_Scroll += m_ImgLength;
	}

	void EndScene(LPDIRECT3DTEXTURE9 texture)
	{
		m_Spr[2]->setTexture(texture);
		loop = false;
	}

	void setSpeedUp(float speedUp, boolean flag) {
		// speed up
		if(flag)
			m_ScrollSpeed *= speedUp;
	}

protected:
	CSprite* m_Spr[3];

	D3DXVECTOR3 m_BasePos;
	float m_Scroll;
	float m_ImgLength; // be width or height
	float m_ScrollSpeed;
	boolean loop;
};

