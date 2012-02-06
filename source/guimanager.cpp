#include <CEGUIDefaultResourceProvider.h>

#include "application.h"
#include "guimanager.h"

using namespace OT;

GUIManager::GUIManager()
{
	window = NULL;
	input  = NULL;
	
	system        = NULL;
	renderer      = NULL;
	windowManager = NULL;
}

GUIManager::~GUIManager()
{
	//TODO: do cleanup here.
}

bool GUIManager::init(sf::RenderWindow * window)
{
	this->window = window;
	this->input  = &window->GetInput();
	
	initializeMaps();
	
	try {
		renderer = new CEGUI::OpenGLRenderer(0, 800, 600);
		system   = new CEGUI::System(renderer);
		
		CEGUI::DefaultResourceProvider* rp = (CEGUI::DefaultResourceProvider*)CEGUI::System::getSingleton().getResourceProvider();
		rp->setResourceGroupDirectory("main", "data/debug/cegui/");
		
		CEGUI::Imageset::setDefaultResourceGroup("main");
		CEGUI::Font::setDefaultResourceGroup("main");
		CEGUI::Scheme::setDefaultResourceGroup("main");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("main");
		CEGUI::WindowManager::setDefaultResourceGroup("main");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_main");
		
		CEGUI::SchemeManager::getSingleton().loadScheme("WindowsLook.scheme");
		CEGUI::FontManager::getSingleton().createFont("DejaVuSans-10.font");
		
		system->setDefaultMouseCursor("WindowsLook", "MouseArrow");
		windowManager = CEGUI::WindowManager::getSingletonPtr();
	} catch (CEGUI::Exception & e) {
		LOG(ERROR, "CEGUI error: %s", e.getMessage().c_str());
		return false;
	}
	return true;
}

bool GUIManager::handleEvent(sf::Event & event)
{
	switch (event.Type) {
	case sf::Event::TextEntered:
		return system->injectChar(event.Text.Unicode);
	case sf::Event::KeyPressed:
		return system->injectKeyDown(toCEGUI(event.Key.Code));
	case sf::Event::KeyReleased:
		return system->injectKeyUp(toCEGUI(event.Key.Code));
	case sf::Event::MouseMoved:
		return system->injectMousePosition(static_cast<float>(input->GetMouseX()), static_cast<float>(input->GetMouseY()));
	case sf::Event::MouseButtonPressed:
		return system->injectMouseButtonDown(toCEGUI(event.MouseButton.Button));
	case sf::Event::MouseButtonReleased:
		return system->injectMouseButtonUp(toCEGUI(event.MouseButton.Button));
	case sf::Event::MouseWheelMoved:
		return system->injectMouseWheelChange(static_cast<float>(event.MouseWheel.Delta));
	}
	return false;
}

void GUIManager::draw()
{
	system->renderGUI();
}

CEGUI::System * GUIManager::getSystem() const { return system; }
CEGUI::WindowManager * GUIManager::getWindowManager() const { return windowManager; }
void GUIManager::setRootWindow(CEGUI::Window * window) { system->setGUISheet(window); }

void GUIManager::initializeMaps()
{
	keyMap[sf::Key::Escape]          = CEGUI::Key::Escape       ;
	keyMap[sf::Key::Num1]            = CEGUI::Key::One          ;
	keyMap[sf::Key::Num2]            = CEGUI::Key::Two          ;
	keyMap[sf::Key::Num3]            = CEGUI::Key::Three        ;
	keyMap[sf::Key::Num4]            = CEGUI::Key::Four         ;
	keyMap[sf::Key::Num5]            = CEGUI::Key::Five         ;
	keyMap[sf::Key::Num6]            = CEGUI::Key::Six          ;
	keyMap[sf::Key::Num7]            = CEGUI::Key::Seven        ;
	keyMap[sf::Key::Num8]            = CEGUI::Key::Eight        ;
	keyMap[sf::Key::Num9]            = CEGUI::Key::Nine         ;
	keyMap[sf::Key::Num0]            = CEGUI::Key::Zero         ;
	keyMap[sf::Key::Dash]            = CEGUI::Key::Minus        ;
	keyMap[sf::Key::Equal]           = CEGUI::Key::Equals       ;
	keyMap[sf::Key::Back]            = CEGUI::Key::Backspace    ;
	keyMap[sf::Key::Tab]             = CEGUI::Key::Tab          ;
	keyMap[sf::Key::Q]               = CEGUI::Key::Q            ;
	keyMap[sf::Key::W]               = CEGUI::Key::W            ;
	keyMap[sf::Key::E]               = CEGUI::Key::E            ;
	keyMap[sf::Key::R]               = CEGUI::Key::R            ;
	keyMap[sf::Key::T]               = CEGUI::Key::T            ;
	keyMap[sf::Key::Y]               = CEGUI::Key::Y            ;
	keyMap[sf::Key::U]               = CEGUI::Key::U            ;
	keyMap[sf::Key::I]               = CEGUI::Key::I            ;
	keyMap[sf::Key::O]               = CEGUI::Key::O            ;
	keyMap[sf::Key::P]               = CEGUI::Key::P            ;
	keyMap[sf::Key::LBracket]        = CEGUI::Key::LeftBracket  ;
	keyMap[sf::Key::RBracket]        = CEGUI::Key::RightBracket ;
	keyMap[sf::Key::Return]          = CEGUI::Key::Return       ;
	keyMap[sf::Key::LControl]        = CEGUI::Key::LeftControl  ;
	keyMap[sf::Key::A]               = CEGUI::Key::A            ;
	keyMap[sf::Key::S]               = CEGUI::Key::S            ;
	keyMap[sf::Key::D]               = CEGUI::Key::D            ;
	keyMap[sf::Key::F]               = CEGUI::Key::F            ;
	keyMap[sf::Key::G]               = CEGUI::Key::G            ;
	keyMap[sf::Key::H]               = CEGUI::Key::H            ;
	keyMap[sf::Key::J]               = CEGUI::Key::J            ;
	keyMap[sf::Key::K]               = CEGUI::Key::K            ;
	keyMap[sf::Key::L]               = CEGUI::Key::L            ;
	keyMap[sf::Key::SemiColon]       = CEGUI::Key::Semicolon    ;
	keyMap[sf::Key::LShift]          = CEGUI::Key::LeftShift    ;
	keyMap[sf::Key::BackSlash]       = CEGUI::Key::Backslash    ;
	keyMap[sf::Key::Z]               = CEGUI::Key::Z            ;
	keyMap[sf::Key::X]               = CEGUI::Key::X            ;
	keyMap[sf::Key::C]               = CEGUI::Key::C            ;
	keyMap[sf::Key::V]               = CEGUI::Key::V            ;
	keyMap[sf::Key::B]               = CEGUI::Key::B            ;
	keyMap[sf::Key::N]               = CEGUI::Key::N            ;
	keyMap[sf::Key::M]               = CEGUI::Key::M            ;
	keyMap[sf::Key::Comma]           = CEGUI::Key::Comma        ;
	keyMap[sf::Key::Period]          = CEGUI::Key::Period       ;
	keyMap[sf::Key::Slash]           = CEGUI::Key::Slash        ;
	keyMap[sf::Key::RShift]          = CEGUI::Key::RightShift   ;
	keyMap[sf::Key::Multiply]        = CEGUI::Key::Multiply     ;
	keyMap[sf::Key::LAlt]            = CEGUI::Key::LeftAlt      ;
	keyMap[sf::Key::Space]           = CEGUI::Key::Space        ;
	keyMap[sf::Key::F1]              = CEGUI::Key::F1           ;
	keyMap[sf::Key::F2]              = CEGUI::Key::F2           ;
	keyMap[sf::Key::F3]              = CEGUI::Key::F3           ;
	keyMap[sf::Key::F4]              = CEGUI::Key::F4           ;
	keyMap[sf::Key::F5]              = CEGUI::Key::F5           ;
	keyMap[sf::Key::F6]              = CEGUI::Key::F6           ;
	keyMap[sf::Key::F7]              = CEGUI::Key::F7           ;
	keyMap[sf::Key::F8]              = CEGUI::Key::F8           ;
	keyMap[sf::Key::F9]              = CEGUI::Key::F9           ;
	keyMap[sf::Key::F10]             = CEGUI::Key::F10          ;
	keyMap[sf::Key::Numpad7]         = CEGUI::Key::Numpad7      ;
	keyMap[sf::Key::Numpad8]         = CEGUI::Key::Numpad8      ;
	keyMap[sf::Key::Numpad9]         = CEGUI::Key::Numpad9      ;
	keyMap[sf::Key::Subtract]        = CEGUI::Key::Subtract     ;
	keyMap[sf::Key::Numpad4]         = CEGUI::Key::Numpad4      ;
	keyMap[sf::Key::Numpad5]         = CEGUI::Key::Numpad5      ;
	keyMap[sf::Key::Numpad6]         = CEGUI::Key::Numpad6      ;
	keyMap[sf::Key::Add]             = CEGUI::Key::Add          ;
	keyMap[sf::Key::Numpad1]         = CEGUI::Key::Numpad1      ;
	keyMap[sf::Key::Numpad2]         = CEGUI::Key::Numpad2      ;
	keyMap[sf::Key::Numpad3]         = CEGUI::Key::Numpad3      ;
	keyMap[sf::Key::Numpad0]         = CEGUI::Key::Numpad0      ;
	keyMap[sf::Key::F11]             = CEGUI::Key::F11          ;
	keyMap[sf::Key::F12]             = CEGUI::Key::F12          ;
	keyMap[sf::Key::F13]             = CEGUI::Key::F13          ;
	keyMap[sf::Key::F14]             = CEGUI::Key::F14          ;
	keyMap[sf::Key::F15]             = CEGUI::Key::F15          ;
	keyMap[sf::Key::RControl]        = CEGUI::Key::RightControl ;
	keyMap[sf::Key::Divide]          = CEGUI::Key::Divide       ;
	keyMap[sf::Key::RAlt]            = CEGUI::Key::RightAlt     ;
	keyMap[sf::Key::Pause]           = CEGUI::Key::Pause        ;
	keyMap[sf::Key::Home]            = CEGUI::Key::Home         ;
	keyMap[sf::Key::Up]              = CEGUI::Key::ArrowUp      ;
	keyMap[sf::Key::PageUp]          = CEGUI::Key::PageUp       ;
	keyMap[sf::Key::Left]            = CEGUI::Key::ArrowLeft    ;
	keyMap[sf::Key::Right]           = CEGUI::Key::ArrowRight   ;
	keyMap[sf::Key::End]             = CEGUI::Key::End          ;
	keyMap[sf::Key::Down]            = CEGUI::Key::ArrowDown    ;
	keyMap[sf::Key::PageDown]        = CEGUI::Key::PageDown     ;
	keyMap[sf::Key::Insert]          = CEGUI::Key::Insert       ;
	keyMap[sf::Key::Delete]          = CEGUI::Key::Delete       ;

	mouseButtonMap[sf::Mouse::Left]     = CEGUI::LeftButton;
	mouseButtonMap[sf::Mouse::Middle]   = CEGUI::MiddleButton;
	mouseButtonMap[sf::Mouse::Right]    = CEGUI::RightButton;
	mouseButtonMap[sf::Mouse::XButton1] = CEGUI::X1Button;
	mouseButtonMap[sf::Mouse::XButton2] = CEGUI::X2Button;
}

CEGUI::Key::Scan GUIManager::toCEGUI(sf::Key::Code code)
{
	KeyMap::iterator i = keyMap.find(code);
	if (i == keyMap.end())
		return (CEGUI::Key::Scan)0;
	return i->second;
}

CEGUI::MouseButton GUIManager::toCEGUI(sf::Mouse::Button button)
{
	MouseButtonMap::iterator i = mouseButtonMap.find(button);
	if (i == mouseButtonMap.end())
		return (CEGUI::MouseButton)0;
	return i->second;
}
