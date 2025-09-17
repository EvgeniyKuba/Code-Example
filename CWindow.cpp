#include "CWindow.h"

//////////////////////////////////////////////////////////////////////////

namespace KGF
{
	//////////////////////////////////////////////////////////////////////////
	CWindow::CWindow()
	{
		setUp();

		window.create(sf::VideoMode({ mainWidth, mainHeight }), s_title, sf::Style::Close);

		if (fullScreen == 1)
		{
			setFullscreen();
		}

		if (!image.loadFromFile(s_ico.toAnsiString()))
		{
			#ifdef DEBUG
				spdlog::warn("Cant't load window pictogram image.");
			#endif
		}
		window.setIcon(image.getSize(), image.getPixelsPtr());
		
	}
	//////////////////////////////////////////////////////////////////////////
	CWindow::~CWindow()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	void CWindow::setFullscreen()
	{
		HWND hwnd = getSFMLWindowHandle(window);
		DWORD style = WS_VISIBLE | WS_POPUP;;
		SetWindowLong(hwnd, GWL_STYLE, style);

		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(hwnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_NOZORDER | SWP_FRAMECHANGED);

		config.setAttribute("Window", "fullscreen", "1");

		fullScreen = 1;

		#ifdef DEBUG
			spdlog::info("Window set fullscreen mode");
		#endif
	}
	//////////////////////////////////////////////////////////////////////////
	void CWindow::setSize(unsigned short width, unsigned short height)
	{
		HWND hwnd = getSFMLWindowHandle(window);
		DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX;
		SetWindowLong(hwnd, GWL_STYLE, style);

		//-- Setting the window position in the center of screen
		rect.left = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		rect.top = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;

		SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, width, height, SWP_NOZORDER | SWP_FRAMECHANGED);

		//
		config.setAttribute("Window", "width", std::to_string(width));
		config.setAttribute("Window", "height", std::to_string(height));
		config.setAttribute("Window", "fullscreen", "0");
		//

		fullScreen = 0;

		if (!image.loadFromFile(s_ico.toAnsiString()))
		{
			#ifdef DEBUG
				spdlog::warn("Can't load window pictogram image.");
			#endif
		}
		window.setIcon(image.getSize(), image.getPixelsPtr());

		#ifdef DEBUG
			spdlog::info("Window set windowed mode. Width = {} Height = {}", width, height);
		#endif
	}
	//////////////////////////////////////////////////////////////////////////
	void CWindow::setUp()
	{
		mainWidth = static_cast<unsigned short>(std::stoul(config.getAttribute("Window", "width")));
		mainHeight = static_cast<unsigned short>(std::stoul(config.getAttribute("Window", "height")));
		s_title = config.getAttribute("Window", "title");
		s_ico = config.getAttribute("Window", "pictogram");
		s_title = sf::String::fromUtf8(s_title.begin(), s_title.end()); // -- For UNICODE in win title

		fullScreen = static_cast<unsigned short>(std::stoul(config.getAttribute("Window", "fullscreen")));
	}
	//////////////////////////////////////////////////////////////////////////
	HWND CWindow::getSFMLWindowHandle(const sf::RenderWindow& window)
	{
		return (HWND)window.getNativeHandle();
	}
	//////////////////////////////////////////////////////////////////////////
}