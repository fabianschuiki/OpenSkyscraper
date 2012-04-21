#pragma once
#include <cmath>

namespace OT {
	
	class Game;
	class Time
	{
		friend class Game;
		
	public:
		const static double kBaseSpeed;
		
		Time();
		
		double absolute;
		double dta;
		double hour;
		int day;
		int quarter;
		int year;
		
		bool checkHour(double t);
		
	protected:
		void set(double t);
		void advance(double dt);
		
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