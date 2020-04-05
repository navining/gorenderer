/**
 * @file gui.cpp
 * @discription GUI window for the renderer.
 * @author Navi Ning <superokvv@outlook.com>
 */

#include <Windows.h>
#include <tchar.h>
#include "Raster.h"
#include "CELLTimestamp.hpp"
#include "test.hpp"

using namespace CELL;

/**
 * Callback function for WinMain().
 */
LRESULT CALLBACK windowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**
 * Main function.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Register window class
	::WNDCLASSEX winClass;
	winClass.lpszClassName = "GoRenderer";
	winClass.cbSize = sizeof(::WNDCLASSEX);
	winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	winClass.lpfnWndProc = windowProc;
	winClass.hInstance = hInstance;
	winClass.hIcon = 0;
	winClass.hIconSm = 0;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)(BLACK_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	RegisterClassExA(&winClass);

	// Creater the window
	HWND hWnd = CreateWindowEx(
		NULL,
		"GoRenderer",
		"GoRenderer",
		WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_THICKFRAME),
		//WS_POPUPWINDOW,
		0,
		0,
		512,
		512,
		0,
		0,
		hInstance,
		0
	);
	UpdateWindow(hWnd);
	ShowWindow(hWnd, SW_SHOW);

	RECT rt = { 0 };
	GetClientRect(hWnd, &rt);

	int width = rt.right - rt.left;
	int height = rt.bottom - rt.top;
	void* buffer = 0;

	HDC hDC = GetDC(hWnd);
	HDC hMem = ::CreateCompatibleDC(hDC);

	// Create an image
	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;

	HBITMAP hBmp = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&buffer, 0, 0);
	SelectObject(hMem, hBmp);

	Raster raster(width, height, buffer);

	// Message Queue
	MSG msg = { 0 };
	while (true) {
		if (msg.message == WM_DESTROY || msg.message == WM_CLOSE || msg.message == WM_QUIT) {
			break;
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		CELLTimestamp timeStamp;
		timeStamp.update();

		// Draw
		raster.clear();
		draw(raster);
		
		// Calculate FPS
		double msec = timeStamp.getElapsedTimeInMicroSec();
		char szBuf[128];
		sprintf_s(szBuf, "FPS: %f ", 1000000.0f / msec);

		TextOut(hMem, 10, 10, szBuf, strlen(szBuf));
		BitBlt(hDC, 0, 0, width, height, hMem, 0, 0, SRCCOPY);
	}
	return 0;
}