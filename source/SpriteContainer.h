#pragma once

namespace OT {
	
	class SpriteContainer
	{
	public:
		std::set<Sprite *> all;
		std::set<Sprite *> visible;
	};
}