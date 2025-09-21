#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
class FPSCounter {
public:
  FPSCounter() : frame_count(0), fps(0), last_time(SDL_GetTicks()) {}

  void frame_rendered() {
    frame_count++;
    Uint32 current_time = SDL_GetTicks();

    if (current_time - last_time >= 1000) {
      fps         = frame_count;
      frame_count = 0;
      last_time   = current_time;
    }
  }

  int getFPS() const {
    return fps;
  }

private:
  int    frame_count;
  int    fps;
  Uint32 last_time;
};
