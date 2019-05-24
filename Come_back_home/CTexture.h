#pragma once
#define MAX_SIZE 50

class CTexture
{
public:
	CTexture(LPDIRECT3DDEVICE9 pD3DDevice);
	virtual ~CTexture();

	int LoadTexture(const TCHAR* filename);
	int LoadTexture(int slot, const TCHAR* filename);
	void ReleaseAll();

	LPDIRECT3DTEXTURE9 GetTexture(int slot) const;

protected:
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	LPDIRECT3DTEXTURE9 m_pTextures[MAX_SIZE];
};

