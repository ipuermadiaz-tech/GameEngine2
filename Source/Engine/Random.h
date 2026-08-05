#pragma once
#include <stdlib.h>

namespace ru {
	inline int RandomInt()
	{
		return rand();
	}
	/// <summary>
	/// Get random integer value[0,max]
	/// </summary>
	/// <param name="max">inclusive max</param>
	/// <returns>returns random number between 0 and max (exclusive)</returns>
	inline int RandomInt(int max)
	{
		return rand() %(max+1);
	}
	/// <summary>
	/// Get random integer value[min,max]
	/// </summary>
	/// <param name="min">the minimum</param>
	/// <param name="max">inclusive max</param>
	/// <returns></returns>
	inline int RandomInt(int min,int max)
	{
		return min +RandomInt(1+max-min);
	}

	inline float RandomFloat() {
		// rand()=0 <- RAND_MAX
		return rand() / (float)RAND_MAX;
	}
	inline float RandomFloat(float max)
	{
		//0 <-> 1 * max
		return RandomFloat() * max;
	}
	inline float RandomFloat(float min,float max)
	{
		//min +(0 <-> 1) * (max-min)
		return min+RandomFloat() * (max-min);
	}


}