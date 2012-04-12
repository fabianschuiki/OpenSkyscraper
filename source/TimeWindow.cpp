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

void TimeWindow::updateTime()
{
	Time & t = game->time;
	
	window->GetElementById("watch")->SetAttribute<float>("time", t.hour);
	
	Rocket::Core::Element * weekday = window->GetElementById("date-weekday");
	Rocket::Core::Element * weekend = window->GetElementById("date-weekend");
	weekday->SetInnerRML(t.day == 0 ? "1st WD" : "2nd WD");
	weekday->SetProperty("display", (t.day == 2 ? "none" : "inline"));
	weekend->SetProperty("display", (t.day != 2 ? "none" : "inline"));
	
	char c[128];
	snprintf(c, 128, "%iQ", t.quarter);
	window->GetElementById("date-quarter")->SetInnerRML(c);
	
	const char * suffix = "th";
	int yl = (t.year % 10);
	if (yl == 1) suffix = "st";
	if (yl == 2) suffix = "nd";
	if (yl == 3) suffix = "rd";
	snprintf(c, 128, "%i%s", t.year, suffix);
	window->GetElementById("date-year")->SetInnerRML(c);
	
	snprintf(c, 128, "%.1f", t.absolute);
	populationDiv->SetInnerRML(c);
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

void TimeWindow::showMessage(std::string msg)
{
	LOG(IMPORTANT, msg.c_str());
	window->GetElementById("message")->SetInnerRML(msg.c_str());
	messageTimer = 5;
}

void TimeWindow::advance(double dt)
{
	if (messageTimer > 0) {
		messageTimer -= dt;
		if (messageTimer <= 0) {
			window->GetElementById("message")->SetInnerRML("");
			messageTimer = 0;
		}
	}
}
