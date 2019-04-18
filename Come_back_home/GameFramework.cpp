#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <time.h>
#include "CTexture.h"
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

	// 
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
	m_ScreenWidth = (float)ClientWidth;
	m_ScreenHeight = (float)ClientHeight;

	D3DPRESENT_PARAMETERS d3dpp;
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
		, &m_pD3DDevice) == E_FAIL)return false;

	srand((unsigned int)time(NULL));

	m_pTexture = NULL;

	return false;
}

void GameFramework::ReleaseFramework()
{
	if (m_pD3DDevice != NULL)
		m_pD3DDevice->Release();
	if (m_pD3D != NULL)
		m_pD3D->Release();

	m_pD3DDevice = NULL;
	m_pD3D = NULL;
}

void GameFramework::LoadTexture()
{
	m_pTexture->LoadTexture(TEXT("../img/pc.png"));
}
