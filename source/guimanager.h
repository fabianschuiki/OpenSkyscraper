#pragma once
#include <CEGUI.h>
#include <openglrenderer.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace OT
{
	class GUIManager
	{
	public:
		GUIManager();
		~GUIManager();
		
		bool init(sf::RenderWindow * window);
		bool handleEvent(sf::Event & event);
		void draw();
		
		CEGUI::System * getSystem() const;
		CEGUI::WindowManager * getWindowManager() const;
		void setRootWindow(CEGUI::Window * window);
		
	private:
		sf::RenderWindow * window;
		const sf::Input * input;
		
		CEGUI::System * system;
		CEGUI::OpenGLRenderer * renderer;
		CEGUI::WindowManager * windowManager;
	 
		typedef std::map<sf::Key::Code, CEGUI::Key::Scan> KeyMap;
		typedef std::map<sf::Mouse::Button, CEGUI::MouseButton> MouseButtonMap;
		
		KeyMap keyMap;
		MouseButtonMap mouseButtonMap;
		
		void initializeMaps();
		
		CEGUI::Key::Scan toCEGUI(sf::Key::Code code);
		CEGUI::MouseButton toCEGUI(sf::Mouse::Button button);
	};
}
