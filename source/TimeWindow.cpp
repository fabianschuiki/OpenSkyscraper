#include <cassert>
#include <Rocket/Core/Element.h>
#include <sstream>
#include "Game.h"
#include "TimeWindow.h"

using namespace OT;
using std::string;


void TimeWindow::close()
{
	ratingDiv     = NULL;
	
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
	assert(ratingDiv);
	
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
	updatePopulation();
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
	Rocket::Core::Element * e = window->GetElementById("funds");
	assert(e);
	e->SetInnerRML(formatMoney(game->funds).c_str());
}

void TimeWindow::updatePopulation()
{
	char c[32];
	snprintf(c, 32, "%i", game->population);
	
	Rocket::Core::Element * e = window->GetElementById("population");
	assert(e);
	e->SetInnerRML(c);
}

void TimeWindow::updateTooltip()
{
	std::stringstream str;
	if (game->toolPrototype) {
		str << "Construct ";
		str << game->toolPrototype->name;
		str << " ";
		str << formatMoney(game->toolPrototype->price);
	} else {
		if (game->selectedTool == "bulldozer") str << "Bulldoze";
		if (game->selectedTool == "finger") str << "Resize elevator shaft";
		if (game->selectedTool == "inspector") str << "Inspect";
	}
	if (!message.empty()) {
		if (!str.str().empty()) str << "  |  ";
		str << message;
	}
	window->GetElementById("message")->SetInnerRML(str.str().c_str());
}

void TimeWindow::showMessage(std::string msg)
{
	LOG(IMPORTANT, msg.c_str());
	message = msg;
	messageTimer = 3;
	updateTooltip();
}

void TimeWindow::advance(double dt)
{
	if (messageTimer > 0) {
		messageTimer -= dt;
		if (messageTimer <= 0) {
			message.clear();
			messageTimer = 0;
			updateTooltip();
		}
	}
}

std::string TimeWindow::formatMoney(int amount)
{
	char c[32];
	snprintf(c, 32, "$%i", amount);
	string fmt(c);
	for (int i = (int)fmt.length() - 3; i > 1; i -= 3) fmt.insert(i, "'");
	return fmt;
}
