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

	void InitGameData();
	void ReleaseGameData();
	
	void GameUpdate(UINT& escapecode);
	void GameRender();

	void Update(float dt);
	void Render();

	void JumpUpdate(float dt);

protected:
	// settings
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	float m_ScreenWidth;
	float m_ScreenHeight;

	// use classes
	CText* m_Text;
	CTexture* m_Texture;
	CInput* m_Input;

	// settings
	bool m_Pause;
	DWORD m_dwPrevTime; // how much times passed
	DWORD m_dwTotalTime; // play time

	int m_GameMode; // gameMode
	int m_Score; // score
	
	
	// game object
	CGameObject* m_Player;
	CGameObject* m_Invader;

	CBackground* m_Background[4]; // background
	CBackground* m_ScoreBar;
	

	// player Jump
	bool m_Jump; // is jumping?
	bool m_Jump2; // is jumping second?
	bool m_JumpFalling; // is falling?
	// float m_fJumpPower; // first jump: , second jump: 8
	float m_fGroundHeight; // ground height
	float m_PrevHeight; // check previous height for know is falling
	float m_JumpTime; // Jump Time
};

