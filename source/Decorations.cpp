/* Copyright © 2013 Fabian Schuiki */
#include "Decorations.h"
#include "Game.h"
using OT::Decorations;


Decorations::Decorations(Game * game)
:	GameObject(game)
{
	// Initialize the crane sprite.
	crane.SetImage(App->bitmaps["simtower/deco/crane"]);
	crane.SetCenter(20, 36);
	craneVisible = false;
}

/** Updates the decorations for the given floor. This effectively repositions the fire stairs of
 * the tower on this floor. */
void Decorations::updateFloor(int y)
{
	// Find all items on the given floor.
	Game::ItemSet items = game->itemsByFloor[y];
	bool none = true;
	int minFloorX = INT_MAX;
	int maxFloorX = INT_MIN;
	if (!items.empty() && y >= 0) {
		for (Game::ItemSet::const_iterator ii = items.begin(); ii != items.end(); ii++) {
			Item::Item * i = *ii;
			if (i->canHaulPeople()) continue;
			minFloorX = std::min(minFloorX, i->position.x);
			maxFloorX = std::max(maxFloorX, i->getRect().maxX());
			none = false;
		}
	}

	// If there are none, remove any existing stairs.
	if (none) {
		FireStairs::iterator i = fireStairs.find(y);
		if (i != fireStairs.end()) {
			fireStairs.erase(i);
			LOG(DEBUG, "Removed fire stairs on floor %i", y);
		}
	}

	// Otherwise calculate the width of the floor.
	else {
		//Create a new pair of fire stairs if required.
		bool exists = fireStairs.count(y);
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
		fsp.minX.SetPosition(minFloorX * 8, -y * 36);
		fsp.maxX.SetPosition(maxFloorX * 8, -y * 36);
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
			crane.SetPosition((minFloorX + maxFloorX) / 2 * 8, -(maxY + 1) * 36);
			craneVisible = (maxFloorX - minFloorX >= 4);
		} else {
			craneVisible = false;
		}
	} else {
		craneVisible = false;
	}
}

/** Renders the tower decorations to the given render target. */
void Decorations::Render(sf::RenderTarget & target) const
{
	sf::FloatRect rect = target.GetView().GetRect();
	for (int y = floor(-rect.Bottom / 36); y <= ceil(-rect.Top / 36); y++) {
		FireStairs::const_iterator fsi = fireStairs.find(y);
		if (fsi == fireStairs.end()) continue;
		target.Draw(fsi->second.minX);
		target.Draw(fsi->second.maxX);
	}

	if (craneVisible) target.Draw(crane);
}