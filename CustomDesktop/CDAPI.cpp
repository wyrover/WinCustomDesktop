﻿#include "stdafx.h"
#include <CDAPI.h>
#include "CDAPIModule.h"
#include "Global.h"
#include <CDEvents.h>


namespace cd
{
	CD_API HWND WINAPI GetTopHwnd()
	{
		return g_global.m_topWnd;
	}

	CD_API HWND WINAPI GetParentHwnd()
	{
		return g_global.m_parentWnd;
	}

	CD_API HWND WINAPI GetFileListHwnd()
	{
		return g_global.m_fileListWnd;
	}

	CD_API void WINAPI GetWndSize(SIZE& size)
	{
		size = g_global.m_wndSize;
	}


	CD_API std::wstring WINAPI GetPluginDir()
	{
		return g_global.m_cdDir + L"Plugin\\";
	}

	CD_API UINT WINAPI GetCustomMessageID()
	{
		static UINT s_nextMsgID = WM_APP + 1000;
		return s_nextMsgID++;
	}

	static const UINT WM_EXEC_FUNCTION = GetCustomMessageID();

	CD_API void WINAPI ExecInMainThread(std::function<void()> function)
	{
		PostMessage(g_global.m_fileListWnd, WM_EXEC_FUNCTION, (WPARAM)new std::function<void()>(std::move(function)), NULL);
	}


	static bool OnFileListWndProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_EXEC_FUNCTION)
		{
			(*(std::function<void()>*)wParam)();
			return false;
		}
		return true;
	}

	CDAPIModule::CDAPIModule()
	{
		g_fileListWndProcEvent.AddListener(OnFileListWndProc);
	}
}