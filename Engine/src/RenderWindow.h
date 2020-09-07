#pragma once
#include "Utils/ErrorLogger.h"

namespace fe
{
	class WindowContainer;

	class RenderWindow
	{
	public:
		bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
		bool ProcessMessages();
		HWND GetHWND() const;
		~RenderWindow();
	private:
		void RegisterWindowClass();
		HWND handle = NULL; //Handle to this window
		HINSTANCE hInstance = NULL; //Handle to application instance
		std::string window_title = "";
		std::wstring window_title_wide = L""; //Wide string representation of window title
		std::string window_class = "";
		std::wstring window_class_wide = L""; //Wide string representation of window class name
	};
}