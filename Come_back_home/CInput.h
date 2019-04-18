#pragma once
class CInput
{
public:
	explicit CInput(HWND hWnd, HINSTANCE hInstance);
	virtual ~CInput();

	int IntInput(HWND hWnd, HINSTANCE hInstance);
	void ShutdownInput();
	bool ReadKeyboard();
	bool IsEscapePressed();
	bool IsPressed(int key); // jump, pause, space
	void GetArrowDIr(D3DXVECTOR3& vecDir);

	float IsKeyPressed(int key) {
		return (m_keyboardState[key] & 0x80) ? 1.0f : 0.0f;
	}

	void clearKeyBuffer() {
		SecureZeroMemory(m_keyboardState, sizeof(m_keyboardState));
	}
protected:
	LPDIRECTINPUT8 m_lpDInput;
	LPDIRECTINPUTDEVICE8 m_lpKeyboard;
	unsigned char m_keyboardState[256];
};

