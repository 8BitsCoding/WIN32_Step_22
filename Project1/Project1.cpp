// Project1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Project1.h"

HWND gh_talk_wnd, gh_edit_wnd;
HWND gh_chat_edit, gh_send_btn;

void FindTalkWindow()
{
	gh_talk_wnd = NULL;
	while (gh_talk_wnd = FindWindowEx(NULL, gh_talk_wnd, L"#32770", NULL)) {
		gh_edit_wnd = FindWindowEx(gh_talk_wnd, NULL, L"RichEdit20W", NULL);
		if (gh_edit_wnd != NULL) {
			break;
		}
	}
}

void SendChatData()
{
	if (gh_edit_wnd == NULL) return;

	wchar_t str[128];

	GetWindowText(gh_chat_edit, str, 128);

	SendMessage(gh_edit_wnd, WM_SETTEXT, 0, (LPARAM)str);
	// SendMessage는 동기식으로 메시지를 즉각 처리
	Sleep(200);		// 너무빨리 엔터를 치면 딜레이를 넣음
	PostMessage(gh_edit_wnd, WM_KEYDOWN, 0x0000000D, 0x001C001);	// 엔터키를 누름
	PostMessage(gh_edit_wnd, WM_KEYDOWN, 0x0000000D, 0xC01C001);	// 엔터키를 누름
	// PostMessage는 메시지 큐에 메시지를 넣는다.(안전)

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) {
		gh_chat_edit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
			5, 303, 532, 28, hWnd, (HMENU)25002, NULL, NULL);

		gh_send_btn = CreateWindow(L"Button", L"전송",
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			540, 301, 80, 31, hWnd, (HMENU)25003, NULL, NULL);
		
		FindTalkWindow();

		return 0;
	}
	else if (uMsg == WM_COMMAND) {
		// 컨트롤을 클릭할시 발생하는 메시지
		if (LOWORD(wParam) == 25003) {
			SendChatData();
		}
	}
	else if (uMsg == WM_DESTROY) {
		DestroyWindow(gh_chat_edit);
		DestroyWindow(gh_send_btn);
		PostQuitMessage(0);
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		//FindTalkWindow();
		//SendChatData();
	}
		
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wchar_t my_class_name[] = L"tipssoft";

	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, L"www.tipssoft.com",
		//WS_OVERLAPPEDWINDOW, 50, 50, 640, 374, NULL, NULL, hInstance, NULL);
		// Resize를 막자
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, 50, 50, 640, 374, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}