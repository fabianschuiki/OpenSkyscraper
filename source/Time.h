#pragma once
#include <cmath>

namespace OT {
	
	class Game;
	class Time
	{
		friend class Game;
		
	public:
		double absolute;
		
		double hour;
		int day;
		int quarter;
		int year;
		
	protected:
		void set(double t);
		void advance(double dt);
	};
}