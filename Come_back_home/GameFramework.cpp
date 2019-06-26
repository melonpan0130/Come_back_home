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
#include "CTrapManager.h"
#include "GameFramework.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// use mysql
// #include <my_global.h>
#include <WinSock2.h>
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

float m_TrapTimerGap[5] = { 800.f, 1200.f,1600.f, 2000.f, 2400.f };
D3DXVECTOR3 ArrowPos[3] = { D3DXVECTOR3(945.f, 570.f, 0), D3DXVECTOR3(945.f, 670.f, 0), D3DXVECTOR3(945.f, 770.f, 0) };
D3DXVECTOR3 SelectArrowPos[2] = { D3DXVECTOR3(550.f,350.f,0), D3DXVECTOR3(1200.f,350.f,0) };

int checkid = 0;
int randomCount = 0;

int ha = 10;
boolean once = true;

GameFramework::GameFramework()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	// initialize classes
	m_Texture = NULL;
	m_Text = NULL;
	for (int i = 0; i < 2; i++)
		m_Background1[i] = NULL;
	for (int i = 0; i < 4; i++)
		m_Background2[i] = NULL;

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
	m_AnimationTexture = 0;
	m_IsAnimationAsc = true;
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
	for(int i=0; i<6; i++)
		m_Title[i] = NULL;

	// background
	for (int i = 0; i < 2; i++)
		m_Background1[i] = NULL;

	for (int i = 0; i < 4; i++)
		m_Background2[i] = NULL;

	m_Raining = NULL;

	// bar
	m_Bar = NULL;

	// life
	for (int i = 0; i < 3; i++)
		m_Life[i] = NULL;

	m_PlayerPM = NULL;
	m_InvaderPM = NULL;
	m_TrapTM = NULL;
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
	// pc
	m_Texture->LoadTexture(0, TEXT("../img/pc/pc1.png"));
	m_Texture->LoadTexture(1, TEXT("../img/pc/pc2.png"));
	m_Texture->LoadTexture(2, TEXT("../img/pc/pc3.png"));
	// 1..
	m_Texture->LoadTexture(3, TEXT("../img/pc/payload1.png"));
	m_Texture->LoadTexture(4, TEXT("../img/pc/payload2.png"));

	// system
	m_Texture->LoadTexture(5, TEXT("../img/system/ready.png"));
	m_Texture->LoadTexture(6, TEXT("../img/system/board.png"));
	m_Texture->LoadTexture(7, TEXT("../img/system/life1.png"));
	m_Texture->LoadTexture(8, TEXT("../img/system/life2.png"));
	m_Texture->LoadTexture(9, TEXT("../img/system/life3.png"));

	// title
	m_Texture->LoadTexture(10, TEXT("../img/title/arrow.png"));
	m_Texture->LoadTexture(11, TEXT("../img/title/main.png"));
	m_Texture->LoadTexture(12, TEXT("../img/title/how_to.png"));
	m_Texture->LoadTexture(13, TEXT("../img/title/score.png"));

	// select subject 
	m_Texture->LoadTexture(14, TEXT("../img/title/subject.png"));
	m_Texture->LoadTexture(15, TEXT("../img/title/subject_arrow.png"));

	// item
	m_Texture->LoadTexture(18, TEXT("../img/item/3way.png"));
	m_Texture->LoadTexture(19, TEXT("../img/item/speedUp.png"));
	m_Texture->LoadTexture(20, TEXT("../img/item/warhead.png"));

	// stage 1
	m_Texture->LoadTexture(21, TEXT("../img/stage1/teacher.png"));
	m_Texture->LoadTexture(22, TEXT("../img/stage1/payload.png"));
	m_Texture->LoadTexture(23, TEXT("../img/stage1/class.png"));
	m_Texture->LoadTexture(24, TEXT("../img/stage1/floor.png"));
	m_Texture->LoadTexture(25, TEXT("../img/stage1/trap1.png"));
	m_Texture->LoadTexture(26, TEXT("../img/stage1/trap2.png"));
	m_Texture->LoadTexture(27, TEXT("../img/stage1/trap3.png"));

	// change Scene
	m_Texture->LoadTexture(28, TEXT("../img/stage1/changeScene.png"));

	// stage 2 - background
	m_Texture->LoadTexture(30, TEXT("../img/stage2/cloud.png"));
	m_Texture->LoadTexture(31, TEXT("../img/stage2/thunder.png"));
	m_Texture->LoadTexture(32, TEXT("../img/stage2/cloudy.png")); // 0
	m_Texture->LoadTexture(33, TEXT("../img/stage2/cloudy_cloud.png")); //1
	m_Texture->LoadTexture(34, TEXT("../img/stage2/building.png")); // 2
	m_Texture->LoadTexture(35, TEXT("../img/stage2/ground.png")); // 3
	m_Texture->LoadTexture(36, TEXT("../img/stage2/rain.png"));

	// stage 2 - trap
	
}

void GameFramework::InitGameData()
{
	m_dwPrevTime = GetTickCount64();
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
		, m_Texture->GetTexture(21)
		, D3DXVECTOR3(150.f, 150.f, 0)
		, D3DXVECTOR3(2070, 350, 0)
		, 300);
	m_Invader->setAlive(true);

	// * title *
	// arrow
	m_TitleArrow = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(10)
		, D3DXVECTOR3(167, 45, 0) // center
		, ArrowPos[0]
		, 0);
	m_TitleArrow->setAlive(true);

	// title
	for(int i=0; i<4; i++)
	m_Title[i] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(i+11)
		, 1920
		, 0);

	m_Ready = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(5)
		, 1920
		, 0);

	// * background1 *
	for (int i = 0; i < 2; i++)
		m_Background1[i] = new CBackground(m_pD3DDevice
			, m_Texture->GetTexture(i + 23)
			, 1920
			, (i == 1) ? 800 : 400);

	// background2
	// cloudy
	m_Background2[0] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(32)
		, 1920
		, 50);

	// cloudy_cloud
	m_Background2[1] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(33)
		, 1920
		, 50);

	// building
	m_Background2[2] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(34)
		, 7559
		, 400);

	// ground
	m_Background2[3] = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(35)
		, 1920
		, 800);

	// raining
	m_Raining = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(36)
		, 1080
		, 500); // vertical true

	// board
	m_Bar = new CBackground(m_pD3DDevice
		, m_Texture->GetTexture(6)
		, 1920
		, 0);

	// life
	for (int i = 0; i < 3; i++) {
		m_Life[i] = new CGameObject(m_pD3DDevice
			, m_Texture->GetTexture(7 + i)
			, D3DXVECTOR3(0, 0, 0)
			, D3DXVECTOR3(0, 0, 0)
			, 0);
		m_Life[i]->setAlive(true);
	}
	
	m_PlayerPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(2)
		, D3DXVECTOR3(35.f, 35.f, 0)
		, 800.f
		, D3DXVECTOR3(0.f, -1.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth, m_ScreenHeight));

	// teacher's project
	m_InvaderPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(22)
		, D3DXVECTOR3(50.f, 50.f, 0)
		, 400
		, D3DXVECTOR3(0.f, 1.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth, 840));

	m_TrapTM = new CTrapManager(m_pD3DDevice
		, m_Texture->GetTexture(25)
		, m_Texture->GetTexture(26)
		, m_Texture->GetTexture(27)
		, D3DXVECTOR3(62.f, 75.f, 0.f)
		, D3DXVECTOR3(119.5, 117.5, 0.f)
		, D3DXVECTOR3(107.f, 60.f, 0.f)
		, 800
		, D3DXVECTOR3(-1.f, 0.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth + 119.5, m_ScreenHeight)
		, -300.f);

	m_ItemPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(18)
		, m_Texture->GetTexture(19)
		, m_Texture->GetTexture(20)
		, D3DXVECTOR3(50, 50, 0)
		, 800
		, D3DXVECTOR3(-1.f, 0.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth + 50, m_ScreenHeight));

	// settings
	m_fGroundHeight = m_Player->getPosition().y;
}

void GameFramework::ReleaseGameData()
{
	// delete bar
	delete m_Bar;

	// delete Background
	for (int i = 0; i < 2; i++)
		delete m_Background1[i];

	for (int i = 0; i < 4; i++)
		delete m_Background2[i];

	delete m_Raining;

	for (int i = 0; i < 6; i++)
		delete m_Title[i];
	
	m_TitleArrow = NULL;

	m_ItemPM = NULL;
	m_TrapTM = NULL;
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
		
		DWORD dwCurTime = GetTickCount64();
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
			case 1: case 2: 
				TitleUpdate(fDt, m_GameMode);
				break; 
			case 8: // select subject
				SelectUpdate(fDt);
				break;
			case 9: // ready
				ReadyUpdate(fDt);
				break;
			case 10: // play game ; stage1
				Update(fDt);
				playerAnimationUpdate(fDt);
				break;
			case 19: // change Scene

				break;
			case 20: // stage2
				Update2(fDt);
				break;
			}
			// �� �߻� update ..
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
		case 1: case 2:
			TitleRender(m_GameMode);
			break;
		case 8:
			// select subject
			SelectRender();
			break;
		case 9:// ready
			ReadyRender();
			break;
		case 10: // stage1
			Render();
			break;
		case 19: // change Scene
			break;
		case 20: // stage2
			Render2();
			break;
		}

		m_pD3DDevice->EndScene();
	}

	// send backbuffer
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void GameFramework::TitleUpdate(float dt)
{
	for(int i=0; i<2; i++)
		m_Title[i]->Update(dt);
	m_TitleArrow->Update(dt);

	if (m_Input->IsPressed(DIK_DOWNARROW) || m_Input->IsPressed(DIK_RIGHTARROW))
	{
		if (m_TitleMode != 2)
			m_TitleMode = (++m_TitleMode % 3);
	}
	else if (m_Input->IsPressed(DIK_UPARROW) || m_Input->IsPressed(DIK_LEFTARROW))
	{
		if (m_TitleMode != 0) {
			m_TitleMode = (m_TitleMode-- % 3);
		}
	}
	m_TitleArrow->setPosition(ArrowPos[m_TitleMode]);
	
	if (m_Input->IsPressed(DIK_SPACE))
	{
		switch (m_TitleMode)
		{
		case 0:
			m_TitleArrow->setTexture(m_Texture->GetTexture(15));
			m_TitleArrow->setPosition(D3DXVECTOR3(0, 0, 0));
			m_TitleMode = 0;
			m_GameMode = 8;
			break;
		case 1: case 2:
			m_GameMode = m_TitleMode;
			break;
		}
	}
}

void GameFramework::TitleRender()
{
	m_Title[0]->Render();

	m_TitleArrow->Render();

	m_pD3DDevice->EndScene();

//	TCHAR szSQL[50];
//	_stprintf_s(szSQL, 50, _T("sql : %s"), mysql_get_client_info());
// 	m_Text->Draw(0, 0, 500, 100, szSQL);
}

void GameFramework::TitleUpdate(float dt, int mode)
{
	m_Title[mode]->Update(dt);

	if (m_Input->IsPressed(DIK_SPACE))
	{
		m_GameMode = 0;
	}
}

void GameFramework::TitleRender(int mode)
{
	m_Title[mode]->Render();

	m_pD3DDevice->EndScene();
}

void GameFramework::SelectUpdate(float dt)
{
	m_Title[3]->Update(dt);
	m_TitleArrow->Update(dt);

	if (m_Input->IsPressed(DIK_DOWNARROW) || m_Input->IsPressed(DIK_RIGHTARROW))
	{
		if (m_TitleMode != 1)
			m_TitleMode = (++m_TitleMode % 2);
	}
	else if (m_Input->IsPressed(DIK_UPARROW) || m_Input->IsPressed(DIK_LEFTARROW))
	{
		if (m_TitleMode != 0) {
			m_TitleMode = (m_TitleMode-- % 2);
		}
	}
	m_TitleArrow->setPosition(SelectArrowPos[m_TitleMode]);

	if (m_Input->IsPressed(DIK_SPACE))
	{
		if (m_TitleMode)m_PlayerPM->setTextureAll(m_Texture->GetTexture(2)); // software
		else m_PlayerPM->setTextureAll(m_Texture->GetTexture(3)); // design

		m_dwStartTime = GetTickCount64();
		m_GameMode = 9;
	}
}

void GameFramework::SelectRender()
{
	m_Title[3]->Render();
	m_TitleArrow->Render();
}

void GameFramework::ReadyUpdate(float dt)
{
	int ready_time = GetTickCount64() - m_dwStartTime;
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

	if (m_fTotalTime > 2.f)
		m_Invader->Update(dt);
	
	if (ha==10&&m_Invader->IsTouched(150.f, m_ScreenWidth - 150.f, m_InvaderRightDir))
	{
		m_Invader->setDirection((m_InvaderRightDir ? -1.f : 1.f), 0.f);
		m_InvaderRightDir = !m_InvaderRightDir;
	}

	PayloadUpdate(dt); // fire payload
	InvaderCollision(dt); // is invader collisioned?
	PlayerCollision(dt); // is player collisioned?
	JumpUpdate(dt); // jump

	// update background
	for (int i = 0; i < 2; i++)
		m_Background1[i]->Update(dt);

	for (int i = 0; i < 3; i++)
		m_Life[i]->Update(dt);
	
	// if 1 minuts passed, change the scene.
	// gamemode = 19
	// test -> gamemode = 20
	float changeScene = 5.f;

	if (m_fTotalTime > changeScene) {
		// change invader
		// m_traptm set texture or change tm..
		m_Background1[0]->EndScene(m_Texture->GetTexture(28));
		// m_GameMode = 20;

		for (int i = 0; i < 4; i++)
			m_Background2[i]->Update(dt); // background2
	}
	
	// change scene and invader
	if (m_fTotalTime > changeScene + 3.f) 
	{
		// change scene
		m_Background1[1]->EndScene(m_Texture->GetTexture(24)); // change ground
		
		// change invader
		changeInvader(dt, m_Texture->GetTexture(30), m_Texture->GetTexture(31));
	}
}

void GameFramework::Render()
{
	// render background
	for (int i = 0; i < 4; i++)
		m_Background2[i]->Render();

	// render background
	for (int i = 0; i < 2; i++)
		m_Background1[i]->Render();

	float changeScene = 0.f;
	if (m_fTotalTime > changeScene) {
		// change invader
		// m_traptm set texture or change tm..
		m_Background1[0]->EndScene(m_Texture->GetTexture(28));
		// m_GameMode = 20;
		m_Background2[0]->Render();
		m_Background2[1]->Render();
		m_Background2[2]->Render();

		m_Background1[0]->Render();// class

		m_Background2[3]->Render(); // ground
		m_Background1[1]->Render(); // floor
	}


	// render bar
	m_Bar->Render();

	for (int i = 0; i < 3; i++)
		m_Life[i]->Render();
	
	// payload
	m_PlayerPM->Draw();
	m_InvaderPM->Draw();
	m_TrapTM->Draw();
	m_ItemPM->Draw();

	// render player
	m_Player->Render();
	m_Invader->Render();


	// draw Score
	TCHAR szScore[50];
	_stprintf_s(szScore, 50, _T("%d"), m_Score);
	m_Text->DrawRT(m_ScreenWidth-600, 70, 500, 100, szScore);

	// test to check
	TCHAR szTest[50];
	_stprintf_s(szTest, 50, _T("m_itemtimer : %0.0f, total play time : %0.4f, %d"), m_ItemTimer[0][3], m_fTotalTime, ha);
	m_Text->Draw(0, 0, 1000, 100, szTest);
}

void GameFramework::Update2(float dt)
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
		m_Background2[i]->Update(dt); // background2
	m_Raining->UpdateVertical(dt); // raining

	for (int i = 0; i < 3; i++)
		m_Life[i]->Update(dt);
}

void GameFramework::Render2()
{
	// render background
	for (int i = 0; i < 4; i++)
		m_Background2[i]->Render();

	// payload
	m_PlayerPM->Draw();
	m_InvaderPM->Draw();
	m_TrapTM->Draw();
	m_ItemPM->Draw();

	// render player
	m_Player->Render();
	m_Invader->Render();

	// raining
	m_Raining->Render();

	// render bar
	m_Bar->Render();

	for (int i = 0; i < 3; i++)
		m_Life[i]->Render();

	// draw Score
	TCHAR szScore[50];
	_stprintf_s(szScore, 50, _T("%d"), m_Score);
	m_Text->DrawRT(m_ScreenWidth - 600, 70, 500, 100, szScore);

	// test to check
	TCHAR szTest[50];
	_stprintf_s(szTest, 50, _T("m_itemtimer : %0.0f, total play time : %0.4f"), m_ItemTimer[0][3], m_fTotalTime);
	m_Text->Draw(0, 0, 1000, 100, szTest);
}

void GameFramework::PayloadUpdate(float dt)
{
	m_PlayerPM->Update(dt);
	m_InvaderPM->Update(dt);
	m_TrapTM->Update(dt);
	m_ItemPM->Update(dt);

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
	int invader_time = GetTickCount64() - m_InvaderShootTimer;
	int trap_time = GetTickCount64() - m_TrapShootTimer;

	// after -> make a level value, and make delta_time faster.
	if (invader_time > 3000)
	{
		m_InvaderShootTimer = GetTickCount64();
		if (m_Invader->getAlive())
			m_InvaderPM->OnFire(m_Invader->getPosition());
	}
	
	if (trap_time > m_TrapGap) // make trap position by random using array
	{	
		m_TrapGap = m_TrapTimerGap[rand() % 5]; // random
		int id = (int)rand() % 6;
		if (id < 3) {
			randomCount++;
			m_ItemPM->OnFire(D3DXVECTOR3(m_ScreenWidth + 50, 850.f, 0.f), id);
		}
		else m_TrapTM->OnFire(D3DXVECTOR3(m_ScreenWidth + 119.5, 950.f, 0.f), id - 3);
		
		checkid = id;
		m_TrapShootTimer = GetTickCount64();
	}

	if (m_ItemSwitch[0]) {
		m_ItemTimer[0][1] = GetTickCount64() - m_ItemTimer[0][0];
		m_ItemTimer[0][3] = GetTickCount64() - m_ItemTimer[0][2];

		if (m_ItemTimer[0][1] > 200)
		{
			m_ItemTimer[0][0] = GetTickCount64(); // time reset
			D3DXVECTOR3 playerPos = m_Player->getPosition();
			// on fire to 5 ways
			m_PlayerPM->OnFire(playerPos, D3DXVECTOR3(0.f, -1.f, 0.f));
			m_PlayerPM->OnFire(playerPos, D3DXVECTOR3(1.f, 0.f, 0.f));
			m_PlayerPM->OnFire(playerPos, D3DXVECTOR3(-1.f, 0.f, 0.f));
			m_PlayerPM->OnFire(playerPos, D3DXVECTOR3(0.7f, -0.7f, 0.f));
			m_PlayerPM->OnFire(playerPos, D3DXVECTOR3(-0.7f, -0.7f, 0.f));
		}

		if (m_ItemTimer[0][3] > 3000) {
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
}

// is player collisioned???
void GameFramework::PlayerCollision(float dt)
{
	if (m_Player->getAlive() &&
		(m_InvaderPM->IsCollision(m_Player->getPosition(), (70 + 50)) != -3))
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

	for(int i=0; i<3; i++)
		if (m_Player->getAlive() && m_TrapTM->IsCollision(m_Player->getPosition(), 70, i))
		{
			m_Score -= 100;
		}
	int id = m_ItemPM->IsCollision(m_Player->getPosition(), (70 + 50));
	if (m_Player->getAlive() && id != -3)
	{
		switch (id)
		{
		case 0: // 3 way
			m_ItemTimer[0][0] = GetTickCount64();
			m_ItemTimer[0][2] = GetTickCount64();
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
			m_JumpPower = 15.0f;
		}
		else if (m_Jump2 == false)
		{
			m_JumpTime = 0.f;
			m_Jump2 = true;
			m_JumpPower = 10.0f;
		}
	}

	if (m_Jump == true)
	{
		m_JumpTime += (dt*4.f);
		float height = m_JumpPower + (-4.9f*(m_JumpTime*m_JumpTime));
		// jumpPower + (gravity * (jumpTime)^2);
		pcPos.y -= height;
		// m_JumpFalling = (pcPos.y - m_PrevHeight < 0.f) ? false : true;
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

void GameFramework::changeInvader(float changeTime, LPDIRECT3DTEXTURE9 texture, LPDIRECT3DTEXTURE9 payload)
{
	float timer = m_fTotalTime - changeTime;

	if(m_Invader->getAlive()==false)
	{ 
		m_Invader->setPosition(D3DXVECTOR3(m_ScreenWidth + 150, 350, 0));
		m_Invader->setTexture(texture);
		m_InvaderPM->setTextureAll(payload);
		m_Invader->setAlive(true);
		ha = 10;
	}
	else if (timer > 3.f && once)
	{ // change scene and invader
		m_Invader->ArrangePosition(-300, m_ScreenWidth + 300); // invader arrage to go out
		ha = 2;
		if (m_Invader->IsTouched(-300))
		{
			m_Invader->setAlive(false);
			once = false;
		}
	}
}

void GameFramework::playerAnimationUpdate(float dt)
{
	int timer = GetTickCount64() - m_AnimationTimer;
	if (timer > 500)
	{
		if (m_AnimationTexture == 0) {
			m_IsAnimationAsc = true;
			m_AnimationTexture++;
		}
		else if (m_AnimationTexture == 2) {
			m_IsAnimationAsc = false;
			m_AnimationTexture--;
		}
		else // m_AnimationTexture == 1
			m_IsAnimationAsc ? m_AnimationTexture++ : --m_AnimationTexture;

		m_Player->setTexture(m_Texture->GetTexture(m_AnimationTexture));
		m_AnimationTimer = GetTickCount64();
	}
	
}