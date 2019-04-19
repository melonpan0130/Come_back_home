#include "stdafx.h"
#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include "CInput.h"

CInput::CInput(HWND hWnd, HINSTANCE hInstance)
{
	m_lpDInput = NULL;
	m_lpKeyboard = NULL;

	InitInput(hWnd, hInstance);
}

CInput::~CInput()
{
	ShutdownInput();
}

int CInput::InitInput(HWND hWnd, HINSTANCE hInstance)
{
	m_lpDInput;
	m_lpKeyboard;
	try {
		auto result = DirectInput8Create(hInstance
			, DIRECTINPUT_VERSION
			, IID_IDirectInput8
			, (LPVOID*)&m_lpDInput
			, NULL);
		if (FAILED(result)) throw 0L;
		result = m_lpDInput->CreateDevice(GUID_SysKeyboard
			, &m_lpKeyboard, NULL);
		if (FAILED(result))throw 1L;
		result = m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))throw 0L;
		result = m_lpKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(result))throw 0L;
		result = m_lpKeyboard->Acquire();
		if (FAILED(result))throw 0L;
	}
	catch (DWORD dwError) 
	{
		LPTSTR errorMsg = NULL;

		switch (dwError) 
		{
		case 0L:
			errorMsg = (LPTSTR)_T("Failed for creating device");
			break;
		case 1L:
			errorMsg = (LPTSTR)_T("Failed for create keyboard");
			break;
		}
		MessageBox(hWnd, errorMsg, _T("Error"), MB_OK);
	}
	return 0;
}

void CInput::ShutdownInput()
{
	if (m_lpKeyboard != NULL)
	{
		m_lpKeyboard->Unacquire();
		m_lpKeyboard->Release();
		m_lpKeyboard = NULL;
	}

	if (m_lpDInput != NULL)
	{
		m_lpDInput->Release();
		m_lpDInput = NULL;
	}
}

bool CInput::ReadKeyboard()
{
	auto result = m_lpKeyboard->GetDeviceState(sizeof(m_keyboardState)
		, (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_lpKeyboard->Acquire();
		else return false;
	}
	return true;
}

bool CInput::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;
	return false;
}

bool CInput::IsPressed(int key)
{
	static DWORD dwPrevTime = 0;
	DWORD dwCurtime = GetTickCount();
	int dt = dwCurtime - dwPrevTime;

	if ((dt > 500) && (m_keyboardState[key] & 0x80))
	{
		m_keyboardState[key] = 0;
		dwPrevTime = dwCurtime;
		return true;
	}
	return false;
}

void CInput::GetArrowDIr(D3DXVECTOR3 & vecDir)
{
	vecDir.x -= IsKeyPressed(DIK_LEFTARROW);
	vecDir.x += IsKeyPressed(DIK_RIGHTARROW);
	vecDir.z = 0.0f;

	D3DXVec3Normalize(&vecDir, &vecDir);
}
