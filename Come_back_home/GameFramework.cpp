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
}


GameFramework::~GameFramework()
{
}

bool GameFramework::InitFramework(HWND hWnd, HINSTANCE hInstance)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)return false;

	RECT rc;
	GetClientRect(hWnd, &rc);

	int ClientWidth = rc.right - rc.left + 1;
	int ClientHeight = rc.bottom - rc.top + 1;
	// m_ScreenWidth = (float)ClientWidth;
	// m_ScreenHeight = (float)ClientHeight;
	m_ScreenWidth = 1920;
	m_ScreenHeight = 1080;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = ClientWidth;
	d3dpp.BackBufferHeight = ClientHeight;

	if (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL
		, hWnd
		, D3DCREATE_HARDWARE_VERTEXPROCESSING
		, &d3dpp
		, &m_pD3DDevice) == E_FAIL) return false;

	srand((unsigned int)time(NULL));

	m_Texture = new CTexture(m_pD3DDevice);
	// m_Input = new CInput(hWnd, hInstance);
	// m_Text = new CText(m_pD3DDevice, 20, 40); // use it later..

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
	m_Texture->LoadTexture(TEXT("../img/pc.png"));
}

void GameFramework::InitGameData()
{
	m_dwPrevTime = GetTickCount();

	m_Player = new CGameObject(m_pD3DDevice
		, m_Texture->GetTexture(0)
		, D3DXVECTOR3(66, 72, 0)
		, D3DXVECTOR3(0, 0, 0)
		, 100);

	
	
}

void GameFramework::ReleaseGameData()
{
	for (int i = 0; i < 3; i++)
		delete m_Background[i];
	delete m_Player;
	m_Player = NULL;
}

void GameFramework::GameUpdate(UINT & escapecode)
{
	if (false)//m_Input->IsEscapePressed())
	{
		escapecode = WM_QUIT;
		return;
	}
	else 
	{
//		m_Input->ReadKeyboard();

//		if (m_Input->IsPressed(DIK_F10))
//			m_Pause = !m_Pause;

		DWORD dwCurTime = GetTickCount();
		DWORD dwDt = dwCurTime - m_dwPrevTime;
		float fDt = (float)(dwDt)*0.001f;
		m_dwPrevTime = dwCurTime;

		if (m_Pause == false)
		{
			// รั น฿ป็
			Update(fDt);
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

	// send backbuffer
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void GameFramework::Update(float dt)
{
	D3DXVECTOR3 pcDir(0.f, 0.f, 0.f);
//	m_Input->GetArrowDIr(pcDir);

	m_Player->setDirection(pcDir.x, pcDir.y);
	m_Player->Update(dt);
	// background update
	// jump update
	// ...
	
}

void GameFramework::Render()
{
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// background render
		m_Player->Render();

		m_pD3DDevice->EndScene();
	}
}
