/* Copyright © 2013 Fabian Schuiki */
#include "Decorations.h"
#include "Game.h"
using OT::Decorations;
using OT::Game;


Decorations::Decorations(Game * game)
:	GameObject(game)
{
	// Initialize the crane sprite.
	crane.SetImage(App->bitmaps["simtower/deco/crane"]);
	crane.SetCenter(20, 36);
	craneVisible = false;

	track.SetImage(App->bitmaps["simtower/metro/tracks"]);
	track.SetSubRect(sf::IntRect(0,0,32,36));
	track.SetCenter(0, 36);
	track.Resize(32, 36);
	tracksVisible = false;
}

/** Updates the decorations for the given floor. This effectively repositions the fire stairs of
 * the tower on this floor. */
void Decorations::updateFloor(int y)
{
	// If there is no floor item, remove any existing stairs.
	if (game->floorItems.count(y) == 0) {
		FireStairs::iterator i = fireStairs.find(y);
		if (i != fireStairs.end()) {
			fireStairs.erase(i);
			LOG(DEBUG, "Removed fire stairs on floor %i", y);
		}
	}

	// Otherwise calculate the width of the floor.
	else {
		Item::Item * f = game->floorItems[y];

		//Create a new pair of fire stairs if required.
		bool exists = fireStairs.count(y) == 1;
		FireStairPair &fsp = fireStairs[y];
		if (!exists) {
			fsp.minX.SetImage(App->bitmaps["simtower/deco/fireladder"]);
			fsp.maxX.SetImage(*fsp.minX.GetImage());
			fsp.minX.SetSubRect(sf::IntRect(0, 0, 24, 36));
			fsp.maxX.SetSubRect(sf::IntRect(24, 0, 48, 36));
			fsp.minX.SetCenter(24, 36);
			fsp.maxX.SetCenter(0, 36);
			LOG(DEBUG, "Added fire stairs on floor %i", y);
		}

		//Position the fire stairs accordingly.
		int minFloorX = f->position.x;
		int maxFloorX = f->getRect().maxX();
		fsp.minX.SetPosition(minFloorX * 8.0f, -y * 36.0f);
		fsp.maxX.SetPosition(maxFloorX * 8.0f, -y * 36.0f);
		LOG(DEBUG, "Repositioned fire stairs on floor %i to %i and %i", y, minFloorX, maxFloorX);
	}
}

/** Updates the position of the crane present at the top of the tower. Call this method whenever
 * the tower's top floor changes, i.e. the tower height is changed or another item is placed on
 * the top floor. */
void Decorations::updateCrane()
{
	//Find the tower's top floor.
	int maxY = 0;
	for (Game::ItemSetByInt::const_iterator ii = game->itemsByFloor.begin(); ii != game->itemsByFloor.end(); ii++) {
		if (ii->first > maxY)
			maxY = ii->first;
	}

	//Find the dimensions of the top floor.
	if (maxY > 0) {
		const Game::ItemSet &items = game->itemsByFloor[maxY];
		int minFloorX = INT_MAX;
		int maxFloorX = INT_MIN;
		for (Game::ItemSet::const_iterator ii = items.begin(); ii != items.end(); ii++) {
			Item::Item * i = *ii;
			minFloorX = std::min(minFloorX, i->position.x);
			maxFloorX = std::max(maxFloorX, i->getRect().maxX());
		}
		if (minFloorX < maxFloorX) {
			crane.SetPosition((minFloorX + maxFloorX) / 2 * 8.0f, -(maxY + 1) * 36.0f);
			craneVisible = (maxFloorX - minFloorX >= 4);
		} else {
			craneVisible = false;
		}
	} else {
		craneVisible = false;
	}
}

void Decorations::updateTracks() {
	if (game->metroStation) {
		track.SetY(-game->metroStation->position.y * 36.0f);
		tracksVisible = true;
	} else {
		tracksVisible = false;
	}
}

/** Renders the tower decorations to the given render target. */
void Decorations::Render(sf::RenderTarget & target) const
{
	sf::FloatRect rect = target.GetView().GetRect();
	for (int y = (int)floor(-rect.Bottom / 36); y <= ceil(-rect.Top / 36); y++) {
		FireStairs::const_iterator fsi = fireStairs.find(y);
		if (fsi == fireStairs.end()) continue;
		target.Draw(fsi->second.minX);
		target.Draw(fsi->second.maxX);
	}

	if (craneVisible) target.Draw(crane);

	if (tracksVisible) {
		sf::FloatRect view = target.GetView().GetRect();
		recti rect = game->metroStation->getRect();

		int minx = (int)floor(view.Left / 32);
		int maxx = 0;
		Sprite t = track;
		if (minx < floor(rect.minX() / 4.0f)) {
			// Render tracks on the left
			maxx = (int) ceil(rect.minX() / 4.0f);
			for (int x = minx; x < maxx; x++) {
				int offl = std::max<int>(0, (int)(view.Left - x*32));
				int offr = std::max<int>(0, (x+1)*4 - rect.minX()) * 8;
				sf::IntRect sr = track.GetSubRect();
				sr.Left  += offl;
				sr.Right -= offr;
				t.SetSubRect(sr);

				t.SetX(x * 32.0f + offl);
				target.Draw(t);

				game->drawnSprites++;
			}
		}

		minx = (int)floor(rect.maxX() / 4.0f);
		if (minx < floor(view.Right/ 32)) {
			// Render tracks on the right
			maxx = (int)ceil(view.Right / 32);
			for (int x = minx; x < maxx; x++) {
				int offl = std::max<int>(0, rect.maxX() - x*4) * 8;
				int offr = std::max<int>(0, (int)((x+1)*32 - view.Right));
				sf::IntRect sr = track.GetSubRect();
				sr.Left  += offl;
				sr.Right -= offr;
				t.SetSubRect(sr);

				t.SetX(x * 32.0f + offl);
				target.Draw(t);
				
				game->drawnSprites++;
			}
		}
	}
}