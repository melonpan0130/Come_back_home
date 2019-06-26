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
class CTrapManager;

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

	// Title
	void TitleUpdate(float dt);
	void TitleRender();

	// Title - more
	void TitleUpdate(float dt, int mode);
	void TitleRender(int mode);

	// Select Subject
	void SelectUpdate(float dt);
	void SelectRender();

	// Ready
	void ReadyUpdate(float dt);
	void ReadyRender();

	// stage 1
	void Update(float dt);
	void Render();

	// change Scene
	void GoOutUpdate(float dt);
	void GoOutRender();

	// stage 2
	void Update2(float dt);
	void Render2();

	void OutroUpdate(float dt);
	void OutroRender();

	void PayloadUpdate(float dt);
	void InvaderCollision(float dt); // is invader collisioned?
	void PlayerCollision(float dt); // is player collisioned?
	//void TrapCollision(float dt);

	void JumpUpdate(float dt);

	void changeInvader(float changeTime, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DTEXTURE9 payload); // in change scene
	void playerAnimationUpdate(float dt);
	
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
	CBackground* m_Title[6]; // title background
	CBackground* m_Ready;
	CBackground* m_Background1[2]; // stage 1's background
	CBackground* m_Background2[4]; // stage 2's background
	CBackground* m_Raining; // stage 2's weather
	CBackground* m_Bar; // score Bar, life Bar
	
	// payloadManager
	CPayloadManager* m_PlayerPM;
	CPayloadManager* m_InvaderPM;
	CPayloadManager* m_TrapPM;
	CPayloadManager* m_ItemPM;
	CTrapManager* m_TrapTM;

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
	float m_ItemTimer[3][4];
	bool m_ItemSwitch[3];
	bool m_speedUpOnce;

	// player Jump
	bool m_Jump; // is jumping?
	bool m_Jump2; // is jumping second?
	bool m_JumpFalling; // is falling?
	float m_fGroundHeight; // ground height
	float m_PrevHeight; // check previous height for know is falling
	float m_JumpTime; // Jump Time
	float m_JumpPower; // Jump Power

	// MYSQL m_MySql;

	// change invader
	boolean m_changeInvStatus;

	//player animation
	bool m_IsAnimationAsc; // image order
	float m_AnimationTimer;
	int m_AnimationTexture;
	boolean m_playerCollisioned;

	// game Difficulty
	// invaderpm timer more shoter
};