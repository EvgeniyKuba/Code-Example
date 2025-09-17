#ifndef C_WINDOW_H
#define C_WINDOW_H

#include "..\service\openfiler\CEngineConfig.h"

namespace KGF
{
	//////////////////////////////////////////////////////////////////////////
	class CWindow
	{
	public:
		CWindow();
		~CWindow();

	public:
		sf::RenderWindow window;

		void setFullscreen();
		void setSize(unsigned short width, unsigned short height);
		
	private:
		CEngineConfig config;

		static HWND getSFMLWindowHandle(const sf::RenderWindow& window);
		RECT rect;

		unsigned short screenWidth, screenHeight, mainWidth, mainHeight, fullScreen;
		sf::String s_title, s_ico;
		sf::Image image; //-- Window pictogram

		void setUp();
	};
}

#endif