#pragma once
class GameFramework
{
public:
	GameFramework();
	virtual ~GameFramework();

	bool InitFramework(HWND hWnd, HINSTANCE hInstance);
	void ReleaseFramework();

	void LoadTexture();

protected:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	float m_ScreenWidth;
	float m_ScreenHeight;

	CTexture* m_pTexture;
};

