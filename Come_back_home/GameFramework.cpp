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
	m_InvaderShootTimer = 0;
}


GameFramework::~GameFramework()
{
}

bool GameFramework::InitFramework(HWND hWnd, HINSTANCE hInstance)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)return false;

	m_ScreenWidth = 1920;
	m_ScreenHeight = 1080;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = 1920;
	d3dpp.BackBufferHeight = 1080;

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
	m_Texture->LoadTexture(TEXT("../img/stage1/invaderPayload.png")); // 12
	m_Texture->LoadTexture(TEXT("../img/stage1/trap1.png")); // 13
}

void GameFramework::InitGameData()
{
	m_dwPrevTime = GetTickCount();
	m_InvaderRightDir = true;

	// player
	m_Player = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(0)
		, D3DXVECTOR3(66, 72, 0)
		, D3DXVECTOR3(100, 836, 0)
		, 400);
	m_Player->setAlive(true);

	// npc
	m_Invader = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(11)
		, D3DXVECTOR3(150.f, 150.f, 0)
		, D3DXVECTOR3(150, 350, 0)
		, 300);
	m_Invader->setAlive(true);

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
		, m_Texture->GetTexture(12)
		, D3DXVECTOR3(50.f, 50.f, 0)
		, 400
		, D3DXVECTOR3(0.f, 1.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth, m_ScreenHeight));

	m_TrapPM = new CPayloadManager(m_pD3DDevice
		, m_Texture->GetTexture(13)
		, D3DXVECTOR3(86.5f, 86.5f, 0.f)
		, 500
		, D3DXVECTOR3(-1.f, 0.f, 0.f)
		, D3DXVECTOR2(m_ScreenWidth+86.5, m_ScreenHeight));

	// settings
	m_fGroundHeight = m_Player->getPosition().y;
	m_Score = 0;
	m_GameMode = 0; // don't use yet...

}

void GameFramework::ReleaseGameData()
{
	// delete bar
	for (int i = 0; i < 2; i++)
		delete m_Bar[i];

	// delete Background
	for (int i = 0; i < 3; i++)
		delete m_Background[i];
	
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
		float fDt = (float)(dwDt)*0.001f;
		m_dwPrevTime = dwCurTime;

		if (m_Pause == false)
		{
			Update(fDt);
			if (m_Input->IsPressed(DIK_SPACE))
				m_PlayerPM->OnFire(m_Player->getPosition());
			// 총 발사 update ..
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

	Render();
	m_Invader->Render();

	// send backbuffer
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void GameFramework::Update(float dt)
{
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
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
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

		m_pD3DDevice->EndScene();
	}

	// draw Score
	TCHAR szScore[50];
	_stprintf_s(szScore, 50, _T("%d"), m_Score);
	m_Text->DrawRT(1700, 50, 150, 100, szScore);
}

void GameFramework::PayloadUpdate(float dt)
{
	m_PlayerPM->Update(dt);
	m_InvaderPM->Update(dt);
	m_TrapPM->Update(dt);

	// invader가 초당 한번씩 쏘게 하기
	int delta_time = GetTickCount() - m_InvaderShootTimer;
	int delta_time_trap = GetTickCount() - m_TrapShootTimer;

	if (delta_time > 5000) // level에 따라서 변화주기
	{
		m_InvaderShootTimer = GetTickCount();
		if (m_Invader->getAlive())
			m_InvaderPM->OnFire(m_Invader->getPosition());
	}

	if (delta_time_trap > 1000) // 1000 -> random, but over than 1000
	{	
		m_TrapShootTimer = GetTickCount();
		m_TrapPM->OnFire(D3DXVECTOR3(m_ScreenWidth+86.5, 900.f, 0.f));
	}
}

void GameFramework::InvaderCollision(float dt)
{
	if (m_Invader->getAlive() &&
		m_PlayerPM->IsCollision(m_Invader->getPosition(), (150.f + 35.f)))
	{
		m_Score += 100;
	}
}

void GameFramework::PlayerCollision(float dt)
{
	if (m_Player->getAlive() &&
		m_InvaderPM->IsCollision(m_Player->getPosition(), (66 + 50)))
	{
		// lose one life
		if (m_Life[0]->getAlive())
			m_Life[0]->setAlive(false);
		else if (m_Life[1]->getAlive())
			m_Life[1]->setAlive(false);
		else if (m_Life[2]->getAlive())
			m_Life[2]->setAlive(false);
		else ;// gamemode = "gameover"
		m_Score -= 100;
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
