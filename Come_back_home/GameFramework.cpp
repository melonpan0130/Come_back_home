#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include "CText.h"
#include "CInput.h"
#include "CTexture.h"
#include "CSprite.h"
#include "CGameObject.h"
#include "CBackground.h"
#include "CPayloadManager.h"
#include "GameFramework.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// use mysql
#include <my_global.h>
#include <WinSock2.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

float m_TrapTimerGap[5] = { 800.f, 1200.f,1600.f, 2000.f, 2400.f };
D3DXVECTOR3 ArrowPos[5] = { D3DXVECTOR3(750.f, 550.f, 0), D3DXVECTOR3(500.f, 750.f, 0), D3DXVECTOR3(1000.f, 750.f, 0), D3DXVECTOR3(500.f, 925.f, 0), D3DXVECTOR3(1000.f, 925.f, 0) };

int checkid = 0;
int randomCount = 0;

GameFramework::GameFramework()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	// initialize classes
	m_Texture = NULL;
	m_Text = NULL;
	for (int i = 0; i < 4; i++)
		m_Background[i] = NULL;

	// basic members
	m_Pause = false;
	m_dwPrevTime = 0;
	m_fTotalTime = 0.f;
	m_InvaderShootTimer = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++)
			m_ItemTimer[i][j] = NULL;
		m_ItemSwitch[0] = false;
	}
		

	m_Score = 0;
	m_GameMode = 0; // don't use yet...
	m_TitleMode = 0;
	m_TrapGap = m_TrapTimerGap[rand() % 5];
	m_InvaderCount = 0;
}

GameFramework::~GameFramework()
{
}

bool GameFramework::InitFramework(HWND hWnd, HINSTANCE hInstance)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
		return false;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = m_ScreenWidth;
	d3dpp.BackBufferHeight = m_ScreenHeight;

	if (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, hWnd
		, D3DCREATE_HARDWARE_VERTEXPROCESSING
		, &d3dpp
		, &m_pD3DDevice) == E_FAIL) return false;

	srand((unsigned int)time(NULL));

	m_Texture = new CTexture(m_pD3DDevice);
	m_Input = new CInput(hWnd, hInstance);
	m_Text = new CText(m_pD3DDevice, 20, 40); // use it later..

	m_Player = NULL; // player
	m_Invader = NULL; // invader

	m_InvaderDir = D3DXVECTOR3(1, 0, 0); // invader direction

	// title
	m_TitleArrow = NULL;
	for(int i=0; i<5; i++)
		m_Title[i] = NULL;


	// background
	for (int i = 0; i < 4; i++)
		m_Background[i] = NULL;

	// bar
	for (int i = 0; i < 2; i++)
		m_Bar[i] = NULL;

	// life
	for (int i = 0; i < 3; i++)
		m_Life[i] = NULL;

	m_PlayerPM = NULL;
	m_InvaderPM = NULL;
	m_TrapPM = NULL;
	m_ItemPM = NULL;
	/*
	mysql_init(&m_MySql);
	// if (!mysql_real_connect(&m_MySql, NULL , "root", "1234", "NULL/mysql/", 3306, NULL, 0))
		// throw 4L;
	mysql_close(&m_MySql);
	*/
	return true;
}

void GameFramework::ReleaseFramework()
{
	delete m_Texture;
	delete m_Input;
	delete m_Text;

	if (m_pD3DDevice != NULL)
		m_pD3DDevice->Release();
	if (m_pD3D != NULL)
		m_pD3D->Release();

	m_pD3DDevice = NULL;
	m_pD3D = NULL;
}

void GameFramework::LoadTexture()
{
	m_Texture->LoadTexture(TEXT("../img/pc.png")); // 0
	m_Texture->LoadTexture(TEXT("../img/stage1/sky.png")); // 1
	m_Texture->LoadTexture(TEXT("../img/stage1/moon.png")); // 2
	m_Texture->LoadTexture(TEXT("../img/stage1/mountain.png")); // 3
	m_Texture->LoadTexture(TEXT("../img/stage1/ground.png")); // 4
	m_Texture->LoadTexture(TEXT("../img/score/scoreBoard.png")); // 5
	m_Texture->LoadTexture(TEXT("../img/score/lifeBoard.png")); // 6
	m_Texture->LoadTexture(TEXT("../img/score/life_first.png")); // 7
	m_Texture->LoadTexture(TEXT("../img/score/life_second.png")); // 8
	m_Texture->LoadTexture(TEXT("../img/score/life_final.png")); // 9
	m_Texture->LoadTexture(TEXT("../img/payload.png")); // 10
	m_Texture->LoadTexture(TEXT("../img/stage1/invader1.png")); // 11
	m_Texture->LoadTexture(TEXT("../img/stage1/invader2.png")); // 12
	m_Texture->LoadTexture(TEXT("../img/stage1/invaderPayload.png")); // 13
	m_Texture->LoadTexture(TEXT("../img/stage1/trap1.png")); // 14
	m_Texture->LoadTexture(TEXT("../img/title/arrow.png")); // 15
	m_Texture->LoadTexture(TEXT("../img/title/main.png")); // 16
	
	// be deleted....
	m_Texture->LoadTexture(TEXT("../img/title/t1.png")); // 17
	m_Texture->LoadTexture(TEXT("../img/title/t2.png")); // 18
	m_Texture->LoadTexture(TEXT("../img/title/t3.png")); // 19
	m_Texture->LoadTexture(TEXT("../img/title/t4.png")); // 20
	m_Texture->LoadTexture(TEXT("../img/title/t5.png")); // 21
	m_Texture->LoadTexture(TEXT("../img/title/how_to.png")); // 22
	m_Texture->LoadTexture(TEXT("../img/title/how_to2.png")); // 23
	m_Texture->LoadTexture(TEXT("../img/title/info.png")); //24
	m_Texture->LoadTexture(TEXT("../img/title/score.png")); // 25
	m_Texture->LoadTexture(TEXT("../img/title/credit.png")); // 26
	
	m_Texture->LoadTexture(TEXT("../img/stage1/ready.png")); // 27
	m_Texture->LoadTexture(TEXT("../img/item/3way.png")); // 28
	m_Texture->LoadTexture(TEXT("../img/item/speedup.png")); // 29
	m_Texture->LoadTexture(TEXT("../img/item/warhead.png")); // 30
}

void GameFramework::InitGameData()
{
	m_dwPrevTime = GetTickCount();
	m_InvaderRightDir = true;

	// player
	m_Player = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(0)
		, D3DXVECTOR3(150, 150, 0)
		, D3DXVECTOR3(100, 790, 0)
		, 400);
	m_Player->setAlive(true);

	// npc
	m_Invader = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(11)
		, D3DXVECTOR3(150.f, 150.f, 0)
		, D3DXVECTOR3(150, 350, 0)
		, 300);
	m_Invader->setAlive(true);

	// * title *
	// arrow
	m_TitleArrow = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(15)
		, D3DXVECTOR3(73, 73, 0)
		, ArrowPos[0]
		, 0);
	m_TitleArrow->setAlive(true);

	// main title
	for(int i=0; i<11; i++)
	m_Title[i] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(i+16)
		, 1920
		, 0);

	m_Ready = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(27)
		, 1920
		, 0);

	// * background *
	// sky
	m_Background[0] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(1)
		, 1920
		, 10);
	
	// moon
	m_Background[1] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(2)
		, 1920
		, 1);

	// mountain
	m_Background[2] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(3)
		, 1920
		, 50);

	// ground
	m_Background[3] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(4)
		, 1920
		, 500);

	// score bar
	m_Bar[0] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(5)
		, 1920
		, 0);

	// life bar
	m_Bar[1] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(6)
		, 1920
		, 0);

	// life
	m_Life[0] = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(7)
		, D3DXVECTOR3(0, 0, 0)
		, D3DXVECTOR3(0, 0, 0)
		, 0);
	
	m_Life[1] = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(8)
		, D3DXVECTOR3(0, 0, 0)
		, D3DXVECTOR3(0, 0, 0)
		, 0);

	m_Life[2] = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(9)
		, D3DXVECTOR3(0, 0, 0)
		, D3DXVECTOR3(0, 0, 0)
		, 0);

	// set life true
	for (int i = 0; i < 3; i++)
		m_Life[i]->setAlive(true);

	m_PlayerPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(10)
		, D3DXVECTOR3(35.f, 35.f, 0)
		, 800.f
		, D3DXVECTOR3(0.f, -1.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth, m_ScreenHeight));

	m_InvaderPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(13)
		, D3DXVECTOR3(50.f, 50.f, 0)
		, 400
		, D3DXVECTOR3(0.f, 1.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth, 840));

	m_TrapPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(14)
		, D3DXVECTOR3(86.5f, 86.5f, 0.f)
		, 500
		, D3DXVECTOR3(-1.f, 0.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth+86.5, m_ScreenHeight));

	m_ItemPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(28)
		, m_Texture->GetTexture(29)
		, m_Texture->GetTexture(30)
		, D3DXVECTOR3(50, 50, 0)
		, 500
		, D3DXVECTOR3(-1.f, 0.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth + 50, m_ScreenHeight));

	// settings
	m_fGroundHeight = m_Player->getPosition().y;

	
	
}

void GameFramework::ReleaseGameData()
{
	// delete bar
	for (int i = 0; i < 2; i++)
		delete m_Bar[i];

	// delete Background
	for (int i = 0; i < 3; i++)
		delete m_Background[i];

	for (int i = 0; i < 10; i++)
		delete m_Title[i];
	
	m_TitleArrow = NULL;

	m_ItemPM = NULL;
	m_TrapPM = NULL;
	m_PlayerPM = NULL;
	m_InvaderPM = NULL;

	delete m_Player; // delete player;
	m_Player = NULL;

	delete m_Invader;
	m_Invader = NULL;
}

void GameFramework::GameUpdate(UINT & escapecode)
{
	if (m_Input->IsEscapePressed())
	{
		escapecode = WM_QUIT;
		return;
	}
	else 
	{
		m_Input->ReadKeyboard();

		if (m_Input->IsPressed(DIK_F10))
			m_Pause = !m_Pause;
		
		DWORD dwCurTime = GetTickCount();
		DWORD dwDt = dwCurTime - m_dwPrevTime;
		DWORD dwPt = dwCurTime - m_dwStartTime;
		float fDt = (float)(dwDt)*0.001f;
		m_dwPrevTime = dwCurTime;

		if (m_Pause == false)
		{
			switch (m_GameMode) {
			case 0:
				TitleUpdate(fDt);
				break;
			case 1: case 2: case 3: case 4: case 5:
				TitleUpdate(fDt, m_GameMode);
				break; 
			case 9: // ready
				ReadyUpdate(fDt);
				break;
			case 10: // play game
				Update(fDt);
				break;
			}
			// รั น฿ป็ update ..
		}
	}
}

void GameFramework::GameRender()
{
	if (m_pD3DDevice == NULL)
		return;

	// clear backbuffer
	m_pD3DDevice->Clear(0, NULL
		, D3DCLEAR_TARGET
		, D3DCOLOR_XRGB(0, 0, 0)
		, 1.0f, 0);
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		
		switch (m_GameMode) {
		case 0:
			TitleRender();
			break;
		case 1: case 2: case 3: case 4: case 5:
			TitleRender(m_GameMode);
			break;
		case 9:// ready
			ReadyRender();
			break;
		case 10:
			Render();
			m_Invader->Render();
			break;
		}

		m_pD3DDevice->EndScene();
	}

	// send backbuffer
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void GameFramework::TitleUpdate(float dt)
{
	for(int i=0; i<6; i++)
		m_Title[i]->Update(dt);
	m_TitleArrow->Update(dt);

	if (m_Input->IsPressed(DIK_DOWNARROW) || m_Input->IsPressed(DIK_RIGHTARROW))
	{
		if (m_TitleMode != 4)
			m_TitleMode = (++m_TitleMode % 5);
	}
	else if (m_Input->IsPressed(DIK_UPARROW) || m_Input->IsPressed(DIK_LEFTARROW))
	{
		if (m_TitleMode != 0) {
			m_TitleMode = (m_TitleMode-- % 5);
		}
	}
	m_TitleArrow->setPosition(ArrowPos[m_TitleMode]);

	if (m_Input->IsPressed(DIK_RETURN))
	{
		switch (m_TitleMode)
		{
		case 0:
			m_dwStartTime = GetTickCount();
			m_GameMode = 9;
			break;
		case 1: 
			m_GameMode = m_TitleMode;
			break;
		case 2: case 3: case 4:
			m_GameMode = m_TitleMode + 1;
		}
	}
}

void GameFramework::TitleRender()
{
	for (int i = 0; i < 6; i++)
		m_Title[i]->Render();

	m_TitleArrow->Render();

	m_pD3DDevice->EndScene();

//	TCHAR szSQL[50];
//	_stprintf_s(szSQL, 50, _T("sql : %s"), mysql_get_client_info());
// 	m_Text->Draw(0, 0, 500, 100, szSQL);
}

void GameFramework::TitleUpdate(float dt, int mode)
{
	m_Title[mode + 5]->Update(dt);

	if (mode == 1 && m_Input->IsPressed(DIK_TAB))
		m_GameMode = 2;

	if (m_Input->IsPressed(DIK_RETURN))
	{
		m_GameMode = 0;
	}
}

void GameFramework::TitleRender(int mode)
{
	m_Title[mode + 5]->Render();

	m_pD3DDevice->EndScene();
}

void GameFramework::ReadyUpdate(float dt)
{
	int ready_time = GetTickCount() - m_dwStartTime;
	if (ready_time > 2000) {
		m_GameMode = 10;
	}
	m_Ready->Update(dt);
}

void GameFramework::ReadyRender()
{
	Render();
	m_Ready->Render();
}

void GameFramework::Update(float dt)
{
	m_fTotalTime += dt;
	D3DXVECTOR3 pcDir(0.f, 0.f, 0.f);
	m_Input->GetArrowDIr(pcDir);

	m_Player->setDirection(pcDir.x, pcDir.y);
	m_Player->ArrangePosition(66.f, m_ScreenWidth - 66.f); // limit moving
	m_Player->Update(dt);

	m_Invader->setDirection(m_InvaderDir.x, m_InvaderDir.y);
	m_Invader->Update(dt);
	
	if (m_Invader->IsTouched(150.f, m_ScreenWidth - 150.f, m_InvaderRightDir))
	{
		m_InvaderDir = D3DXVECTOR3((m_InvaderRightDir ? -1.f : 1.f), 0.f, 0.f);
		m_InvaderRightDir = !m_InvaderRightDir;
	}
	
	PayloadUpdate(dt); // fire payload
	InvaderCollision(dt); // is invader collisioned?
	PlayerCollision(dt); // is player collisioned?
	JumpUpdate(dt); // jump

	// update background
	for (int i = 0; i < 4; i++)
		m_Background[i]->Update(dt);

	for (int i = 0; i < 2; i++)
		m_Bar[i]->Update(dt);

	for (int i = 0; i < 3; i++)
		m_Life[i]->Update(dt);
	
}

void GameFramework::Render()
{
	// render background
	for (int i = 0; i < 4; i++)
		m_Background[i]->Render();

	// render bar
	for (int i = 0; i < 2; i++)
		m_Bar[i]->Render();

	for (int i = 0; i < 3; i++)
		m_Life[i]->Render();
		
	// render player
	m_Player->Render();
	m_Invader->Render();

	// payload
	m_PlayerPM->Draw();
	m_InvaderPM->Draw();
	m_TrapPM->Draw();
	m_ItemPM->Draw();

	// draw Score
	TCHAR szScore[50];
	_stprintf_s(szScore, 50, _T("%d"), m_Score);
	m_Text->DrawRT(m_ScreenWidth-510, 45, 500, 100, szScore);

	// test to check
	TCHAR szTest[50];
	// _stprintf_s(szTest, 50, _T("m_itemTimer : %0.0f, itemswitch : s, total play time : %0.4f"),  m_fTotalTime);
	_stprintf_s(szTest, 50, _T("m_itemtimer : %0.0f, total play time : %0.4f"), m_ItemTimer[0][3], m_fTotalTime);
	m_Text->Draw(0, 0, 1000, 100, szTest);
}

void GameFramework::PayloadUpdate(float dt)
{
	m_PlayerPM->Update(dt);
	m_InvaderPM->Update(dt);
	m_TrapPM->Update(dt);
	m_ItemPM->Update(dt);

	// if()

	if (m_Input->IsPressed(DIK_W))
		m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(0.f, -1.f, 0.f));
	if (m_Input->IsPressed(DIK_D))
		m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(1.f, 0.f, 0.f));
	if (m_Input->IsPressed(DIK_A))
		m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(-1.f, 0.f, 0.f));
	if(m_Input->IsPressed(DIK_E))
		m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(0.7f, -0.7f, 0.f));
	if (m_Input->IsPressed(DIK_Q))
		m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(-0.7f, -0.7f, 0.f));

	// make invader one shoot in one seconds. The other too.
	int invader_time = GetTickCount() - m_InvaderShootTimer;
	int trap_time = GetTickCount() - m_TrapShootTimer;

	// after -> make a level value, and make delta_time faster.
	if (invader_time > 3000)
	{
		m_InvaderShootTimer = GetTickCount();
		if (m_Invader->getAlive())
			m_InvaderPM->OnFire(m_Invader->getPosition());
	}
	
	if (trap_time > m_TrapGap) // make trap position by random using array
	{	
		m_TrapGap = m_TrapTimerGap[rand() % 5]; // random
		int id = 0;
		if (id = rand() % 4 < 3) {
			randomCount++;
			m_ItemPM->OnFire(D3DXVECTOR3(m_ScreenWidth + 50, 850.f, 0.f), 0);
		}
		else m_TrapPM->OnFire(D3DXVECTOR3(m_ScreenWidth + 86.5, 900.f, 0.f));
		checkid = id;
		m_TrapShootTimer = GetTickCount();
	}
	/*
	m_ItemTimer[0][2] = GetTickCount() - 
	m_ItemTimer[0][1] = GetTickCount() - m_ItemTimer[0][0];

	if (m_ItemSwitch[0]) {
		if (m_ItemTimer[0][1] > 200)
		{
			m_ItemTimer[0][0] = GetTickCount(); // time reset

		// on fire to 5 ways
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(0.f, -1.f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(1.f, 0.f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(-1.f, 0.f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(0.7f, -0.7f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(-0.7f, -0.7f, 0.f));
		}
		
		if (m_ItemTimer[0][2] > 50000)
			m_ItemSwitch[0] = false;
	}
	*/

	if (m_ItemSwitch[0]) {
		m_ItemTimer[0][1] = GetTickCount() - m_ItemTimer[0][0];
		m_ItemTimer[0][3] = GetTickCount() - m_ItemTimer[0][2];

		if (m_ItemTimer[0][1] > 200)
		{
			m_ItemTimer[0][0] = GetTickCount(); // time reset

			// on fire to 5 ways
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(0.f, -1.f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(1.f, 0.f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(-1.f, 0.f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(0.7f, -0.7f, 0.f));
			m_PlayerPM->OnFire(m_Player->getPosition(), D3DXVECTOR3(-0.7f, -0.7f, 0.f));

		}

		if (m_ItemTimer[0][3] > 5000) {
			// m_itemtimer
			m_ItemSwitch[0] = false;
		}
			
	}
}


void GameFramework::InvaderCollision(float dt)
{
	// invader collisioned?
	if (m_Invader->getAlive() &&
		(m_PlayerPM->IsCollision(m_Invader->getPosition(), (150.f + 35.f)) != -3))
	{
		m_Score += 200;
		m_InvaderCount += 1;
	}

	if (m_InvaderCount > 20)
	{
		// get out, and appear new invader
		m_Invader->setTexture(m_Texture->GetTexture(12));
	}

	// invaderPM collisioned?
	for (int i = 0; i < 10; i++)
		if (m_InvaderPM->getAlive(i) &&
			(m_PlayerPM->IsCollision(m_InvaderPM->getPos(i), (50.f + 35.f)) != -3))
		{
			m_Score += 100;
			m_InvaderPM->setAlive(i, false);
		}

	// trap collisioned?
	for (int i = 0; i < 10; i++)
		if (m_TrapPM->getAlive(i) &&
			(m_PlayerPM->IsCollision(m_TrapPM->getPos(i), (86.5 + 35.f)) != -3))
		{
			m_Score += 100;
			m_TrapPM->setAlive(i, false);
		}
}

// is player collisioned???
void GameFramework::PlayerCollision(float dt)
{
	if (m_Player->getAlive() &&
		(m_InvaderPM->IsCollision(m_Player->getPosition(), (102.5 + 50)) != -3))
	{
		// lose one life
		if (m_Life[0]->getAlive())
			m_Life[0]->setAlive(false);
		else if (m_Life[1]->getAlive())
			m_Life[1]->setAlive(false);
		else if (m_Life[2]->getAlive())
			m_Life[2]->setAlive(false);
		else {
			// m_Player.setAlive(false);
			// or
			// gameover.setAlive(true);
			// m_GameMode = 50; // gameover mode
		}
		m_Score -= 200;
	}

	if (m_Player->getAlive() && m_TrapPM->IsCollision(m_Player->getPosition(), (102.5 + 86.5)) != -3)
	{
		m_Score -= 100;
	}

	int id = m_ItemPM->IsCollision(m_Player->getPosition(), (102.5 + 50));
	if (m_Player->getAlive() && id != -3)
	{
		switch (id)
		{
		case 0: // 3 way
			m_ItemTimer[0][0] = GetTickCount();
			m_ItemTimer[0][2] = GetTickCount();
			m_ItemSwitch[0] = true;
			break;
		case 1: // speedup
			break;
		case 2: // warhead
			break;
		}
	}
}

void GameFramework::JumpUpdate(float dt)
{
	D3DXVECTOR3 pcPos = m_Player->getPosition();
	if (m_Input->IsPressed(DIK_UPARROW))
	{
		if (m_Jump == false)
		{
			m_Jump = true;
			m_Jump2 = false;
			m_JumpTime = 0.f;
			m_PrevHeight = pcPos.y;
		}
		else if (m_JumpFalling == true && m_Jump2 == false)
		{
			m_JumpTime = 0.f;
			m_Jump2 = true;
		}
	}

	if (m_Jump == true)
	{
		m_JumpTime += (dt*4.f);
		float height = 15.f + (-4.9f*(m_JumpTime*m_JumpTime));
		// jumpPower + (gravity * (jumpTime)^2);
		pcPos.y -= height;
		m_JumpFalling = (pcPos.y - m_PrevHeight < 0.f) ? false : true;
		// check is falling

		// is lower than ground?
		if (pcPos.y > m_fGroundHeight)
		{
			pcPos.y = m_fGroundHeight;
			m_Jump = false;
		}
		m_PrevHeight = pcPos.y; // insert the height of 
		m_Player->setPosition(pcPos);
	}
}
