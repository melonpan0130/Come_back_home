#pragma once
class CGameObject
{
public:
	CGameObject();
	CGameObject(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 pTexture
		, const D3DXVECTOR3& center
		, const D3DXVECTOR3& position
		, float speed = 100.f);
	virtual ~CGameObject();

	void InitGameObject(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 pTexture
		, const D3DXVECTOR3& center
		, const D3DXVECTOR3& position
		, float speed = 100.f);
	void Release();
	
	void Update(float dt);
	void Render();

	void ArrangePosition(float sx, float ex);
	bool IsTouched(float leftX, float rightX, bool useRight);

	void setPosition(D3DXVECTOR3 pos) { m_Pos = pos; }
	const D3DXVECTOR3& getPosition()const { return m_Pos; }

	void setDirection(float x, float y) { m_Dir = D3DXVECTOR3(x, y, 0.f); }
	const D3DXVECTOR3& getDirection()const { return m_Dir; }

	void setAlive(bool flag) { m_Alive = flag; }
	bool getAlive() { return m_Alive; }
protected:
	CSprite* m_Spr;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Center;
	D3DXVECTOR3 m_Dir;

	bool m_Alive;
	float m_Speed;
};

