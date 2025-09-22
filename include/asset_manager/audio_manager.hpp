#pragma once

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>

#include "asset_manager.hpp"

enum class GameAudioChannel { FREE_CHANNEL = -1, WALK_ON_GRASS = 0, FOREST_AMBIENCE = 1, SWORD_MISS_ATTACK = 2 };

struct GameChannelSpecification {
  GameAudioChannel channel;
  std::string      path;
  int              volume;
  bool             loop;
};

namespace Managers {
  constexpr int LOOP_INFINITE = -1;

  class AudioManager : AssetManager<Mix_Chunk*> {
  public:
    static AudioManager& instance();

    void register_channel(const GameChannelSpecification& channel) {
      channels[channel.channel] = channel;
      Mix_Volume(static_cast<int>(channel.channel), channel.volume);
    }

    void       init(int    frequency = 44100,
                    Uint16 format    = MIX_DEFAULT_FORMAT,
                    int    channels  = 2,
                    int    chunksize = 2048);
    void       shutdown();
    void       play_sound(const GameAudioChannel& channel = GameAudioChannel::FREE_CHANNEL);
    void       stop_channel(GameAudioChannel channel);
    bool       channel_playing(GameAudioChannel channel);
    Mix_Chunk* get_or_load(const std::string& path) override;

  private:
    AudioManager() = default;
    ~AudioManager() {}
    AudioManager(const AudioManager&)            = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    std::unordered_map<GameAudioChannel, GameChannelSpecification> channels;
  };

}; // namespace Managers