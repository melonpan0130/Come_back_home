#pragma once
class CText;
class CInput;
class CTexture;
class CSprite;
class CGameObject;
class CBackground;

class GameFramework
{
public:
	GameFramework();
	virtual ~GameFramework();

	bool InitFramework(HWND hWnd, HINSTANCE hInstance);
	void ReleaseFramework();

	// Textures
	void LoadTexture();
	void ReleaseTexture();

	void InitGameData();
	void ReleaseGameData();
	
	void GameUpdate(UINT& escapecode);
	void GameRender();

	void Update(float dt);
	void Render();

	void JumpUpdate();

protected:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	float m_ScreenWidth;
	float m_ScreenHeight;

	// use classes
	CText* m_Text;
	CTexture* m_Texture;
	CInput* m_Input;

	bool m_Pause;
	DWORD m_dwPrevTime; // how much times passed
	DWORD m_dwTotalTime; // play time
	
	// game object
	CGameObject* m_Player;
	CGameObject* m_Invader;

	CBackground* m_Background[4]; // background
};

