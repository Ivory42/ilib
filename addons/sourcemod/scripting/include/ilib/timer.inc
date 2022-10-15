#if defined _ilib_timer_included
 #endinput
#endif
#define _ilib_timer_included

/*
* Struct used to create pseudo timers
* Used with GetGameTime() to determine if the timer has expired
*/
enum struct FTimer
{
	float nextTime; // Game time when this timer will expire next
	float time; // Timer value to use for this timer

	bool looping; // Whether or not this timer is looping

	/*
	* Sets the timer value and activates this timer
	*
	* @param1 time - Duration for this timer
	* @param2 loop - Should this timer loop
	*/
	void Set(float time, bool loop = false)
	{
		this.nextTime = GetGameTime() + time;
		this.time = time;
		this.looping = loop;
	}

	// Clears and deactivates this timer
	void Clear()
	{
		this.nextTime = -1.0;
		this.looping = false;
	}
	
	// Returns the timer remaining on this timer in seconds, or 0 if the timer is not active
	float GetTimeRemaining()
	{
		float result = this.nextTime - GetGameTime();
		if (result)
			return result;
		else
			return 0.0;
	}

	// Returns the length of this timer
	float GetTimerDuration()
	{
		return this.time;
	}

	// Loops this timer with the last given time input
	void Loop()
	{
		this.nextTime = GetGameTime() + this.time;
	}

	/*
	* Returns whether or not this timer has expired
	* If the timer is marked to loop, it will automatically reset when this is called. Otherwise FTimer::Loop() can be used to restart the timer
	*/
	bool Expired()
	{
		bool result = (this.nextTime <= GetGameTime() && this.nextTime != -1.0);

		if (this.looping && result)
			this.nextTime = GetGameTime() + this.time;

		return result;
	}
}

/*
* Creates a timer and returns its information as an FTimer
*
* @param1 duration - Duration for this timer
* @param2 loop - Should this timer loop
*
* @return - FTimer containing this timer's information
*/

stock any[sizeof FTimer] ConstructTimer(float duration, bool loop = false)
{
	FTimer timer;
	timer.Set(duration, loop);

	return timer;
}