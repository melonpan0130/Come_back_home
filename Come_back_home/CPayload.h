#pragma once
class CSprite;

class CPayload
{
public:
	explicit CPayload(LPDIRECT3DDEVICE9 pD3DDevice
	,LPDIRECT3DTEXTURE9& texture
	,const D3DXVECTOR3& center
	,float speed
	,const D3DXVECTOR3& dir
	,const D3DXVECTOR2 screensize
	,float MinX = 0);
	virtual ~CPayload();

	bool IsAlive() const { return m_Alive; }
	bool GetAlive() { return m_Alive; }
	void SetAlive(bool flag) { m_Alive = flag; }

	const D3DXVECTOR3& GetPos() { return m_Pos; }

	void SetDir(const D3DXVECTOR3& dir) { m_Dir = dir; }
	void SetTexture(const LPDIRECT3DTEXTURE9 texture);
	void OnFire(const D3DXVECTOR3& pos)
	{
		m_Alive = true;
		m_Pos = pos;
	}

	void OnFire(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir)
	{
		m_Alive = true;
		m_Pos = pos;
		D3DXVec3Normalize(&m_Dir, &dir);
	}

	void Update(float dt);
	void Draw();
	void setSpeedUp(float speedUp, bool flag)
	{
		if (flag)
			m_Speed *= speedUp;
		else
			m_Speed = m_speedUp;

	}

protected:
	CSprite* m_Spr; // payload
	D3DXVECTOR3 m_Pos; // the game object position; payload started in
	D3DXVECTOR3 m_Dir; // where payload goes?
	D3DXVECTOR2 m_ScreenSize; // payload flies in this screensize

	float m_Speed;
	float m_speedUp;
	float m_MinX;
	bool m_Alive;
};

