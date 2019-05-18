#pragma once
#define PAYLOAD_MAX 40
class CPayload;

class CPayloadManager
{
public:
	explicit CPayloadManager(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 texture
		, const D3DXVECTOR3& center
		, float speed
		, const D3DXVECTOR3& dir
		, const D3DXVECTOR2& screensize);

	explicit CPayloadManager(LPDIRECT3DDEVICE9 pD3DDevice
		, LPDIRECT3DTEXTURE9 texture1
		, LPDIRECT3DTEXTURE9 texture2
		, LPDIRECT3DTEXTURE9 texture3
		, const D3DXVECTOR3& center
		, float speed
		, const D3DXVECTOR3& dir
		, const D3DXVECTOR2& screensize);
	virtual ~CPayloadManager();

	int getEmptyPayload();

	void Update(float dt);
	void Draw();

	void OnFire(const D3DXVECTOR3& pos);
	void OnFire(const D3DXVECTOR3& pos, int type);
	void OnFire(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir);

	int IsCollision(const D3DXVECTOR3& pos, float radious);
	
	const D3DXVECTOR3& getPos(int slot) const;

	bool getAlive(int slot);
	void setAlive(int slot, bool flag);

protected:
	CPayload* m_Payload[PAYLOAD_MAX];
	LPDIRECT3DTEXTURE9 m_Texture[3];

	int m_Type[PAYLOAD_MAX];
};

