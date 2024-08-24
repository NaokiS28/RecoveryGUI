/*
 * BemaniUX - Copyright (C) 2022-2024 spicyjpeg, NaokiS
 *
 * BemaniUX is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BemaniUX is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * BemaniUX. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include "main/uibase.hpp"
#include "windowproc.hpp"
#include <windows.h>

namespace win
{
	HWND Window::CreateMainWindow(int hRes, int vRes)
	{
		HWND windowHandle;
		const char CLASS_NAME[] = "BUX MainWindow";

		WNDCLASS wc = {};

		wc.lpfnWndProc = MainWindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = CLASS_NAME;
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

		RegisterClass(&wc);
		RECT rect = {0, 0, hRes, vRes};
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		// Create the window.
		windowHandle = CreateWindowEx(
			0,					 // Optional window styles.
			CLASS_NAME,			 // Window class
			"BemaniUX",			 // Window text
			WS_OVERLAPPEDWINDOW, // Window style

			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT,
			rect.right - rect.left, rect.bottom - rect.top,

			NULL,				   // Parent window
			NULL,				   // Menu
			GetModuleHandle(NULL), // Instance handle
			NULL				   // Additional application data
		);

		if (windowHandle == NULL)
		{
			LOG_APP("Window creation failed: Window was null");
		}
		else
		{
			// Setup a timer for periodic update/refresh
			SetWindowLongPtr(windowHandle, GWLP_USERDATA, (long long)this);
			ShowWindow(windowHandle, SW_SHOW);
		}

		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(BUX_APP_ICON));
		if (hIcon)
		{
			SendMessage(windowHandle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			SendMessage(windowHandle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		}
		
		return windowHandle;
	}

	LRESULT Window::MainWindowHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		// Mouse
		case WM_SETCURSOR:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
		{
			return uiCtx->ioCtx.mouse.processMessage(hwnd, uMsg, wParam, lParam);
		}

		// Keyboard
		case WM_KEYUP:
		case WM_KEYDOWN:
		{
			return uiCtx->ioCtx.keyboard.processMessage(hwnd, uMsg, wParam, lParam);
		}
		case WM_SIZE:
		{
			RECT rect;
			if (GetWindowRect(hwnd, &rect))
			{
				// int width = rect.right - rect.left;
				// int height = rect.bottom - rect.top;
				//  Dont remake the window as the scale factor will throw this off
				// uiCtx->gpuCtx.setResolution(width, height, false);
			}
			return 0;
		}
		case WM_CREATE:
		{

			break;
		}
		case WM_PAINT:
		{
			uiCtx->draw();
			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;
		case WM_TIMER:
			if (wParam == ui::UI_REFRESH_TIMER)
			{
				// Update the menu or any other UI components.
				uiCtx->runUpdate = true;
			}
			break;
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

		return 0;
	}
}