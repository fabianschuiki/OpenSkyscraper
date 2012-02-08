#include "Application.h"
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
	
	if (exitCode == 0) init();
	if (exitCode == 0) loop();
	if (exitCode == 0) cleanup();
	
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
	
	if (!gui.init(&window)) {
		LOG(ERROR, "unable to initialize gui");
		exitCode = -1;
		return;
	}
	
	if (!monoFont.LoadFromFile(dataDir.down("fonts").down("UbuntuMono-Regular.ttf").c_str(), 14)) {
		LOG(WARNING, "unable to load mono font");
	}
	
	//DEBUG: load some GUI
	Path rocket = dataDir.down("debug").down("rocket");
	Rocket::Core::FontDatabase::LoadFontFace(rocket.down("Delicious-Bold.otf").c_str());
	Rocket::Core::FontDatabase::LoadFontFace(rocket.down("Delicious-BoldItalic.otf").c_str());
	Rocket::Core::FontDatabase::LoadFontFace(rocket.down("Delicious-Italic.otf").c_str());
	Rocket::Core::FontDatabase::LoadFontFace(rocket.down("Delicious-Roman.otf").c_str());
	
	Rocket::Core::ElementDocument * cursor = gui.context->LoadMouseCursor(rocket.down("cursor.rml").c_str());
	if (cursor) {
		cursor->RemoveReference();
	}
	
	Rocket::Core::ElementDocument * document = gui.context->LoadDocument(rocket.down("demo.rml").c_str());
	if (document) {
		document->Show();
		document->RemoveReference();
	}
}

void Application::loop()
{
	sf::Clock clock;
	sf::String rateIndicator("<not available>", monoFont, 14);
	double rateIndicatorTimer = 0;
	double rateDamped = 0;
	double rateDampFactor = 0;
	
	while (window.IsOpened() && exitCode == 0) {
		double dt_real = clock.GetElapsedTime();
		double dt = std::max<double>(dt_real, 0.1); //avoids FPS dropping below 10 Hz
		clock.Reset();
		
		//Update the rate indicator.
		rateDampFactor = (dt_real * 1);
		rateDamped = (rateDamped + dt_real * rateDampFactor) / (1 + rateDampFactor);
		if ((rateIndicatorTimer += dt_real) > 0.1) {
			rateIndicatorTimer -= 0.1;
			char rate[32];
			snprintf(rate, 32, "%.1f Hz", 1.0/rateDamped);
			rateIndicator.SetText(rate);
		}
		
		sf::Event event;
		while (window.GetEvent(event)) {
			switch (event.Type) {
			case sf::Event::Resized:
				LOG(INFO, "resized (%i, %i)", window.GetWidth(), window.GetHeight());
				sf::View view;
				view.SetFromRect(sf::FloatRect(0, 0, window.GetWidth(), window.GetHeight()));
				window.SetView(view);
				break;
			}
			if (gui.handleEvent(event))
				continue;
			switch (event.Type) {
			case sf::Event::Closed:
				exitCode = 1;
				break;
			}
		}
		window.Clear();
		gui.draw();
		window.Draw(rateIndicator);
		window.Display();
	}
	window.Close();
}

void Application::cleanup()
{
}
