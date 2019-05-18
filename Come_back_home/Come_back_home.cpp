// Come_back_home.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <time.h>
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "GameFramework.h"
#include "Come_back_home.h"
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, HWND&, bool);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

GameFramework* g_pGameframework = NULL;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    MyRegisterClass(hInstance);
	HWND hWnd = NULL;
	g_pGameframework = new GameFramework();

    // 응용 프로그램 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow, hWnd, false))
    {
        return FALSE;
    }

	g_pGameframework->InitFramework(hWnd, hInstance);
	g_pGameframework->LoadTexture();
	g_pGameframework->InitGameData();

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMEBACKHOME));

    MSG msg;
	ZeroMemory(&msg, sizeof(msg));

    // 기본 메시지 루프입니다:
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		g_pGameframework->GameUpdate(msg.message);
		g_pGameframework->GameRender();
	}
	g_pGameframework->ReleaseGameData();
	g_pGameframework->ReleaseFramework();
	
	delete g_pGameframework;

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMEBACKHOME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
    wcex.lpszClassName  = TEXT("HOME");
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, HWND& rhWnd, bool fullScreen)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = NULL;

   if (fullScreen) {
	   hWnd = CreateWindowW(TEXT("HOME")
		   , TEXT("Come back home")
		   , WS_EX_TOPMOST | WS_POPUP
		   , 0, 0, 1920, 1080
		   , nullptr, nullptr, hInstance, nullptr);

	   g_pGameframework->m_ScreenWidth = 2400;
	   g_pGameframework->m_ScreenHeight = 1350;
   }
   else {
	   hWnd = CreateWindowW(TEXT("HOME")
		   , TEXT("Come back home")
		   , WS_OVERLAPPEDWINDOW
		   , 0, 0, 1280, 720// 1280, 720
		   , nullptr, nullptr, hInstance, nullptr);
	   g_pGameframework->m_ScreenWidth = 1920;
	   g_pGameframework->m_ScreenHeight = 1080;
   }

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   rhWnd = hWnd;

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}