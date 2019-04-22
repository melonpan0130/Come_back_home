#pragma once
class CText;
class CInput;
class CTexture;
class CSprite;
class CGameObject;
class CBackground;
class CPayloadManager;

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

	void PayloadUpdate(float dt);
	void InvaderCollision(float dt); // is invader collisioned?
	void PlayerCollision(float dt); // is player collisioned?

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
	CGameObject* m_Player; // player
	CGameObject* m_Invader; // invader
	CGameObject* m_Life[3]; // life (3)

	CBackground* m_Background[4]; // background
	CBackground* m_Bar[2]; // score Bar, life Bar
	
	// payloadManager
	CPayloadManager* m_PlayerPM;
	CPayloadManager* m_InvaderPM;
	CPayloadManager* m_TrapPM;

	D3DXVECTOR3 m_InvaderPos;
	D3DXVECTOR3 m_InvaderDir;
	float m_InvaderShootTimer; // invader shoot timer
	bool m_InvaderRightDir;

	float m_TrapShootTimer;

	// player Jump
	bool m_Jump; // is jumping?
	bool m_Jump2; // is jumping second?
	bool m_JumpFalling; // is falling?
	float m_fGroundHeight; // ground height
	float m_PrevHeight; // check previous height for know is falling
	float m_JumpTime; // Jump Time
};

