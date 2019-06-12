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
	virtual ~CTrapManager();

	int getEmptyPayload(int type);

	void Update(float dt);
	void Draw();

	void OnFire(const D3DXVECTOR3& pos, int type);

	bool IsCollision(const D3DXVECTOR3& pos, float radious);
	bool IsCollision(const D3DXVECTOR3& pos, float radious, int type);

protected:
	CPayload* m_Payload[3][PAYLOAD_MAX];
	float m_Size[2][3];
};

