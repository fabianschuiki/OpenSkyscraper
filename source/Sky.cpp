#include "Application.h"
#include "Game.h"
#include "Sky.h"
#include "Sprite.h"

using namespace OT;


void Sky::advance(double dt)
{
	//Decide whether we're about to have a rainy day.
	if (game->time.checkHour(5)) {
		rainyDay = ((rand() % 3) == 0); //Statistically, every 3rd day will be rainy.
		rainAnimation = 0;
		game->timeWindow.showMessage(rainyDay ? "I've heard we're in for some bad weather..." : "Weather's going to be good today!");
	}
	
	/* Decide what sky color to use based on the current time of day. The drawing code will
	 * interpolate between the sky texture offsets 'from' and 'to'. So to get a transition
	 * from day to dusk, from = 0, to = 1 and progress is anywhere between 0..1. */
	double time = game->time.hour;
	
	//Night
	if (time < 5 || time >= 19) {
		from = to = 2; progress = 0;
	}
	
	//Dawn
	else if (time >= 5 && time < 6) {
		from = 2; to = 1; progress = (time-5);
	}
	else if (time >= 6 && time < 7) {
		from = 1; to = 0; progress = (time-6);
	}
	
	//Dusk
	else if (time >= 17 && time < 18) {
		from = 0; to = 1; progress = (time-17);
	}
	else if (time >= 18 && time < 19) {
		from = 1; to = 2; progress = (time-18);
	}
	
	//Day
	else if (!rainyDay) {
		from = to = 0; progress = 0;
	}
	
	//Rain
	else if (time >= 7 && time < 8) {
		from = 0; to = 3; progress = (time-7);
	}
	else if (time >= 16 && time < 17) {
		from = 3; to = 0; progress = (time-16);
	}
	else {
		rainAnimation += dt;
		from = to = 4+floor(fmod(rainAnimation, 1) * 2); progress = 0;
	}
}

void Sky::Render(sf::RenderTarget & target) const
{
	sf::FloatRect rect = target.GetView().GetRect();
	
	//Draw the sky color.
	int sky_lower = std::max<int>(floor(-rect.Bottom / 360), -1);
	int sky_upper = std::min<int>(ceil (-rect.Top    / 360), 11);
	
	Sprite sky;
	sky.SetImage(app->bitmaps["simtower/sky"]);
	for (int y = sky_lower; y <= sky_upper; y++) {
		int index = (std::min<int>(y + 1, 9) * 6 + from);
		sky.SetSubRect(sf::IntRect(index * 32, 0, index * 32 + 32, 360));
		sky.Resize(32, 360);
		sky.SetCenter(0, 360);
		
		for (int x = floor(rect.Left / 32); x < ceil(rect.Right / 32); x++) {
			sky.SetPosition(x * 32, -y * 360);
			target.Draw(sky);
			game->drawnSprites++;
		}
	}
	
	//Draw the skyline, if in view.
	if (-rect.Bottom <= 96 && -rect.Top >= 0) {
		Sprite city;
		city.SetImage(app->bitmaps["simtower/deco/skyline"]);
		city.SetCenter(0, 55);
		for (int x = floor(rect.Left / 96); x < ceil(rect.Right / 96); x++) {
			city.SetPosition(x * 96, 0);
			target.Draw(city);
			game->drawnSprites++;
		}
	}
}
