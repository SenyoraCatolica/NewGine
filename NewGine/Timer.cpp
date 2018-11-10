// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetPerformanceCounter();
	started_new_frame = started_at;
	frequency = SDL_GetPerformanceFrequency();
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}
float Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}

void Timer::PlayGame()
{
	if (is_pause)
	{
		is_pause = false;
		game_paused_time = SDL_GetPerformanceCounter() - game_pause_start;
	}

	else
		game_paused_time = 0;

	game_played_time = 0;
	game_play_start = SDL_GetPerformanceCounter();
}

void Timer::PauseGame()
{
	is_pause = true;
	game_pause_start = SDL_GetPerformanceCounter();
}

void Timer::StopGame()
{
	is_pause = false;

	game_played_time = SDL_GetPerformanceCounter() - game_play_start;
	game_paused_time = 0;
}

void Timer::UpdateTime()
{
	real_time = ((double(SDL_GetPerformanceCounter() - started_new_frame) / double(frequency)));
	started_new_frame = SDL_GetPerformanceCounter();
	if (real_time > 0 && is_play)
	{
		game_time = real_time;
	}
}

