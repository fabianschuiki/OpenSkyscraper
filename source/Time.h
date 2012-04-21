#pragma once
#include <cmath>

namespace OT {
	
	class Game;
	class Time
	{
		friend class Game;
		
	public:
		Time();
		
		int speedMode;
		
		double absolute;
		double hour;
		int day;
		int quarter;
		int year;
		
		bool checkHour(double t);
		
	protected:
		void set(double t);
		void advance(double dt);
		
		void setSpeedMode(int sm);
		
		double prev_absolute;
		double prev_hour;
		int prev_day;
		int prev_quarter;
		int prev_year;
		
	private:
		double speed;
		double speed_animated;
	};
}