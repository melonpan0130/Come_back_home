#pragma once

#include <WinSock2.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

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

	void TitleUpdate(float dt);
	void TitleRender();

	void TitleUpdate(float dt, int mode);
	void TitleRender(int mode);

	void IntroUpdate(float dt);
	void IntroRender();

	void ReadyUpdate(float dt);
	void ReadyRender();

	void Update(float dt);
	void Render();

	void OutroUpdate(float dt);
	void OutroRender();

	void PayloadUpdate(float dt);
	void InvaderCollision(float dt); // is invader collisioned?
	void PlayerCollision(float dt); // is player collisioned?
	//void TrapCollision(float dt);

	void JumpUpdate(float dt);

	float m_ScreenWidth;
	float m_ScreenHeight;

protected:
	// settings
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	// use classes
	CText* m_Text;
	CTexture* m_Texture;
	CInput* m_Input;

	int m_TitleMode;

	// settings
	bool m_Pause;
	DWORD m_dwPrevTime; // how much times passed
	DWORD m_dwStartTime; // start playing
	float m_fTotalTime; // play time

	int m_GameMode; // gameMode
	int m_Score; // score
	
	
	// game object
	CGameObject* m_Player; // player
	CGameObject* m_Invader; // invader
	CGameObject* m_Life[3]; // life (3)
	CGameObject* m_TitleArrow; // title arrow

	// background; 
	CBackground* m_Title[11]; // title background
	CBackground* m_Ready;
	CBackground* m_Background[4]; // background
	CBackground* m_Bar[2]; // score Bar, life Bar
	
	// payloadManager
	CPayloadManager* m_PlayerPM;
	CPayloadManager* m_InvaderPM;
	CPayloadManager* m_TrapPM;
	CPayloadManager* m_ItemPM;

	// invader
	D3DXVECTOR3 m_InvaderPos;
	D3DXVECTOR3 m_InvaderDir;
	float m_InvaderShootTimer; // Invader shoot timer
	bool m_InvaderRightDir; // Is invader going right?
	int m_InvaderCount; // How much damage to invader?

	// trap
	float m_TrapShootTimer;
	float m_TrapGap;
	
	// item
	float m_ItemTimer[3][3];
	bool m_ItemSwitch[3];

	// player Jump
	bool m_Jump; // is jumping?
	bool m_Jump2; // is jumping second?
	bool m_JumpFalling; // is falling?
	float m_fGroundHeight; // ground height
	float m_PrevHeight; // check previous height for know is falling
	float m_JumpTime; // Jump Time

	// MYSQL m_MySql;
};