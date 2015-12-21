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
	crane.setOrigin(20, 36);
	craneVisible = false;

	track.SetImage(App->bitmaps["simtower/metro/tracks"]);
	track.setTextureRect(sf::IntRect(0,0,32,36));
	track.setOrigin(0, 36);
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
			fsp.maxX.SetImage(*fsp.minX.getTexture());
			fsp.minX.setTextureRect(sf::IntRect(0, 0, 24, 36));
			fsp.maxX.setTextureRect(sf::IntRect(24, 0, 24, 36));
			fsp.minX.setOrigin(24, 36);
			fsp.maxX.setOrigin(0, 36);
			LOG(DEBUG, "Added fire stairs on floor %i", y);
		}

		//Position the fire stairs accordingly.
		int minFloorX = f->getRect().minX();
		int maxFloorX = f->getRect().maxX();
		fsp.minX.setPosition(minFloorX * 8.0f, -y * 36.0f);
		fsp.maxX.setPosition(maxFloorX * 8.0f, -y * 36.0f);
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
			crane.setPosition((minFloorX + maxFloorX) / 2 * 8.0f, -(maxY + 1) * 36.0f);
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
		track.setPosition(track.getPosition().x, -game->metroStation->position.y * 36.0f);
		tracksVisible = true;
	} else {
		tracksVisible = false;
	}
}

void Decorations::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	render(target);
}

/** Renders the tower decorations to the given render target. */
void Decorations::render(sf::RenderTarget & target) const
{
	const sf::View &view = target.getView();
	sf::Vector2f center = view.getCenter();
	sf::Vector2f size = view.getSize();
	sf::Vector2f dmax = center + size/2.f;
	sf::Vector2f dmin = center - size/2.f;

	sf::FloatRect rect = target.getView().getViewport();
	for (int y = (int)floor(dmin.y / 36); y <= ceil(dmax.y / 36); y++) {
		FireStairs::const_iterator fsi = fireStairs.find(-y);
		if (fsi == fireStairs.end()) continue;
		target.draw(fsi->second.minX);
		target.draw(fsi->second.maxX);
	}

	if (craneVisible) target.draw(crane);

	if (tracksVisible) {
		// sf::FloatRect view = target.getView().getViewport();
		recti rect = game->metroStation->getRect();

		int minx = (int)floor(dmin.x / 32);
		int maxx = 0;
		Sprite t = track;
		if (minx < floor(rect.minX() / 4.0f)) {
			// Render tracks on the left
			maxx = (int) ceil(rect.minX() / 4.0f);
			for (int x = minx; x < maxx; x++) {
				int offl = std::max<int>(0, (int)(dmin.x - x*32));
				int offr = std::max<int>(0, (x+1)*4 - rect.minX()) * 8;
				sf::IntRect sr = track.getTextureRect();
				sr.left  += offl;
				sr.width -= offl+offr;
				t.setTextureRect(sr);

				t.setPosition(x * 32.0f + offl, t.getPosition().y);
				target.draw(t);

				game->drawnSprites++;
			}
		}

		minx = (int)floor(rect.maxX() / 4.0f);
		if (minx < floor(dmax.x/ 32)) {
			// Render tracks on the right
			maxx = (int)ceil(dmax.x / 32);
			for (int x = minx; x < maxx; x++) {
				int offl = std::max<int>(0, rect.maxX() - x*4) * 8;
				int offr = std::max<int>(0, (int)((x+1)*32 - dmax.x));
				sf::IntRect sr = track.getTextureRect();
				sr.left  += offl;
				sr.width -= offl+offr;
				t.setTextureRect(sr);

				t.setPosition(x * 32.0f + offl, t.getPosition().y);
				target.draw(t);

				game->drawnSprites++;
			}
		}
	}
}
