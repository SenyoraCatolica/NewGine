#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	void PlayGame();
	void PauseGame();
	void StopGame();

	void UpdateTime();

	Uint32 Read();
	float ReadSec()const;

private:

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;


	//Game states
	bool is_play = false;
	bool is_pause = false;
	bool is_stop = true;

	//real time
	unsigned long real_time = 0;

	//game time
	unsigned long game_time = 0;
	unsigned long game_played_time = 0;
	unsigned long game_paused_time = 0;

};

#endif //__TIMER_H__