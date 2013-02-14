#pragma once
#include <cmath>

namespace OT {
	class Game;
	
	/** Manages the game's time progression and scaling.
	 * The following table lists a basic absolute time to hour conversion:
	 * 
	 *  time  absolute
	 *  ----  --------
	 *  0:00  0        0.000
	 *  1:00  1/26    ~0.039
	 *  2:00  2/39    ~0.051
	 *  3:00  5/78    ~0.064
	 *  4:00  1/13    ~0.077
	 *  5:00  7/78    ~0.090
	 *  6:00  4/39    ~0.103
	 *  7:00  3/26    ~0.115
	 *  8:00  19/130  ~0.146
	 *  9:00  23/130  ~0.177
	 * 10:00  27/130  ~0.208
	 * 11:00  31/130  ~0.238
	 * 12:00  7/26    ~0.269
	 * 13:00  15/26   ~0.577
	 * 14:00  8/13    ~0.615
	 * 15:00  17/26   ~0.654
	 * 16:00  9/13    ~0.692
	 * 17:00  19/26   ~0.731
	 * 18:00  10/13   ~0.769
	 * 19:00  21/26   ~0.808
	 * 20:00  11/13   ~0.846
	 * 21:00  23/26   ~0.885
	 * 22:00  12/13   ~0.923
	 * 23:00  25/26   ~0.962
	 *  0:00  1        1
	 */
	class Time
	{
		friend class Game;
		
	public:
		const static double kBaseSpeed;
		static double absoluteToHour(double a);
		static double hourToAbsolute(double a);
		
		Time();
		
		double absolute;
		double dta; //absolute time delta
		double hour;
		int day;
		int quarter;
		int year;
		
		bool check(double a);
		bool checkTick(double p);
		bool checkHour(double h);
		
	protected:
		void set(double a);
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