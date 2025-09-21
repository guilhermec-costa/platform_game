#pragma once

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>

#include "asset_manager.hpp"

namespace Managers {
  class AudioManager : AssetManager<Mix_Chunk*> {
  public:
    static AudioManager& instance();

    void       init(int    frequency = 44100,
                    Uint16 format    = MIX_DEFAULT_FORMAT,
                    int    channels  = 2,
                    int    chunksize = 2048);
    void       shutdown();
    void       play_sound(const std::string& path, int loops = 0);
    void       stop_channel(int channel);
    int        play_loop(const std::string& path);
    Mix_Chunk* get_or_load(const std::string& path) override;

  private:
    AudioManager() = default;
    ~AudioManager() {}
    AudioManager(const AudioManager&)            = delete;
    AudioManager& operator=(const AudioManager&) = delete;
  };
}; // namespace Managers