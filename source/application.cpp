#include "application.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace OT;
using namespace std;

Application * OT::App = NULL;

Application::Application(int argc, char * argv[])
{
	assert(App == NULL && "Application initialized multiple times");
	App = this;
	
	assert(argc >= 1 && "argv[0] is required");
	setPath(argv[0]);
	
	//Special debug defaults.
#ifdef BUILD_DEBUG
	logger.setLevel(Logger::DEBUG);
	char logname[128];
	snprintf(logname, 128, "debug-%li.log", (long int)time(NULL));
	logger.setOutputPath(/*dir.down(*/logname/*)*/);
#endif
	
	//Parse command line arguments.
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "--debug") == 0) {
			logger.setLevel(Logger::DEBUG);
		}
		if (strcmp(argv[i], "--log") == 0) {
			assert(i+1 < argc && "--log is missing path");
			logger.setOutputPath(argv[i++]);
		}
	}
	
	LOG(DEBUG,
		"constructed\n"
		"    path     = %s\n"
		"    dataDir  = %s\n"
		"    prefsDir = %s",
		path.str().c_str(),
		dataDir.str().c_str(),
		prefsDir.str().c_str()
	);
	LOG(IMPORTANT, "ready");
}

Path Application::getPath()     const { return path;     }
Path Application::getDataDir()  const { return dataDir;  }
Path Application::getPrefsDir() const { return prefsDir; }

void Application::setPath(const Path & p)
{
#ifdef __APPLE__
	path     = Path("../MacOS").down(p.name());
	dataDir  = ".";
	prefsDir = "~/Library/Preferences";
#else
	path     = p;
	dataDir  = p.up().down("data");
#endif
}

/** Runs the application. */
int Application::run()
{
	running = true;
	exitCode = 0;
	
	init();
	loop();
	cleanup();
	
	running = false;
	
	if (exitCode < 0) {
		LOG(ERROR, "exitCode = %i", exitCode);
	} else {
		LOG(INFO,  "exitCode = %i", exitCode);
	}
	
	return exitCode;
}

void Application::init()
{
	videoMode.Width        = 800;
	videoMode.Height       = 600;
	videoMode.BitsPerPixel = 32;
	
	window.Create(videoMode, "OpenSkyscraper SFML");
	
	gui.init(&window);
	
	//DEBUG: create some CEGUI interface.
	window.ShowMouseCursor(false);
	try {
		CEGUI::WindowManager* WindowMgr = gui.getWindowManager();
		CEGUI::Window* Dialog = WindowMgr->createWindow("WindowsLook/FrameWindow", "OurDialog");
		Dialog->setMinSize(CEGUI::UVector2(CEGUI::UDim(0.0f, 200),CEGUI::UDim(0.0f, 150)));
		Dialog->setSize(CEGUI::UVector2(CEGUI::UDim(0.0f, 400),CEGUI::UDim(0.0f, 300)));
		Dialog->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.1f, 0)));
		Dialog->setText("Window");
		//Dialog->subscribeEvent(FrameWindow::EventCloseClicked, Event::Subscriber(&App::onDialog_Closed, this));
		CEGUI::Window* EditBox = WindowMgr->createWindow("WindowsLook/Editbox", "OurDialog_Editbox");
		EditBox->setMinSize(CEGUI::UVector2(CEGUI::UDim(0.0f, 100), CEGUI::UDim(0.0f, 20)));
		EditBox->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.1f, 0)));
		EditBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25f, 0), CEGUI::UDim(0.4f, 0)));
		//EditBox->subscribeEvent(CEGUI::Window::EventTextChanged, Event::Subscriber(&App::onEditbox_TextChanged, this));
		Dialog->addChildWindow(EditBox);
		gui.setRootWindow(Dialog);
	} catch (CEGUI::Exception&  e) {
		LOG(ERROR, "CEGUI error: %s", e.getMessage().c_str());
		//return false;
	}
}

void Application::loop()
{
	while (window.IsOpened()) {
		sf::Event event;
		while (window.GetEvent(event)) {
			if (gui.handleEvent(event))
				continue;
			switch (event.Type) {
			case sf::Event::Closed:
				window.Close();
				break;
			}
		}
		window.Clear();
		gui.draw();
		window.Display();
	}
}

void Application::cleanup()
{
	
}
