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
	double started_new_frame;


	//Game states
	bool is_play = false;
	bool is_pause = false;

public:

	//real time
	unsigned long real_time = 0;

	//game time
	double game_time = 0;

	double game_played_time = 0;
	double game_play_start = 0;

	double game_paused_time = 0;
	double game_pause_start = 0;

	double frequency = 0;

};

#endif //__TIMER_H__