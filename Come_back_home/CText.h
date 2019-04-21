#pragma once
class CText
{
public:
	explicit CText(LPDIRECT3DDEVICE9 pD3DDevice
		, int width, int height);
	virtual ~CText();

	void Draw(int x, int y, int width, int height, const TCHAR* lpszText
		, D3DCOLOR color = D3DCOLOR_XRGB(255, 0,0));

	void DrawRT(int x, int y, int width, int height, const TCHAR* lpszText
		, D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));

protected:
	LPD3DXFONT m_pFont;
};

