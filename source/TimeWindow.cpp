#include <cassert>
#include <Rocket/Core/Element.h>
#include <string>
#include "Game.h"
#include "TimeWindow.h"

using namespace OT;
using std::string;


void TimeWindow::close()
{
	ratingDiv     = NULL;
	fundsDiv      = NULL;
	populationDiv = NULL;
	
	if (window) {
		window->RemoveReference();
		window->Close();
	}
	window = NULL;
}

void TimeWindow::reload()
{
	close();
	
	window = game->gui.loadDocument("time.rml");
	assert(window);
	window->Show();
	
	ratingDiv     = window->GetElementById("rating");
	fundsDiv      = window->GetElementById("funds");
	populationDiv = window->GetElementById("population");
	assert(ratingDiv && fundsDiv && populationDiv);
	
	std::string style;
	for (int i = 0; i < 6; i++) {
		char c[128];
		snprintf(c, 128, "div#rating.r%i { background-image-t: %ipx %ipx; }", i, i*22, i*22+22);
		style += c;
	}
	Rocket::Core::StyleSheet * sheet = Rocket::Core::Factory::InstanceStyleSheetString(style.c_str());
	window->SetStyleSheet(window->GetStyleSheet()->CombineStyleSheet(sheet));
	
	updateRating();
	updateFunds();
}

void TimeWindow::updateRating()
{
	for (int i = 0; i < 6; i++) {
		char c[8];
		snprintf(c, 8, "r%i", i);
		ratingDiv->SetClass(c, game->rating == i);
	}
}

void TimeWindow::updateFunds()
{
	char c[32];
	snprintf(c, 32, "$%i", game->funds);
	string fmt(c);
	for (int i = (int)fmt.length() - 3; i > 1; i -= 3) fmt.insert(i, "'");
	fundsDiv->SetInnerRML(fmt.c_str());
}
