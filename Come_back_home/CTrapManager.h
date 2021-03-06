#pragma once
#define PAYLOAD_MAX 10
class CPayload;

class CTrapManager
{
public:
	explicit CTrapManager(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 texture1
		, LPDIRECT3DTEXTURE9 texture2
		, LPDIRECT3DTEXTURE9 texture3
		, const D3DXVECTOR3& center
		, float speed
		, const D3DXVECTOR3 dir
		, const D3DXVECTOR2 screensize);

	explicit CTrapManager(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 texture1
		, LPDIRECT3DTEXTURE9 texture2
		, LPDIRECT3DTEXTURE9 texture3
		, const D3DXVECTOR3& center1
		, const D3DXVECTOR3& center2
		, const D3DXVECTOR3& center3
		, float speed
		, const D3DXVECTOR3 dir
		, const D3DXVECTOR2 screensize
		, float MinX = 0);

	explicit CTrapManager(LPDIRECT3DDEVICE9 pD3DDevice
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
		, float MinX);
	virtual ~CTrapManager();

	int getEmptyPayload(int type);

	void Update(float dt);
	void Draw();

	void OnFire(const D3DXVECTOR3& pos, int type);

	bool IsCollision(const D3DXVECTOR3& pos, float radious);
	bool IsCollision(const D3DXVECTOR3& pos, float radious, int type);

	void setSpeedUp(float speedUp, bool flag);
	void setDeadAll();

protected:
	CPayload* m_Payload[5][PAYLOAD_MAX];
	float m_Size[2][5];

	int textureNum;
};

