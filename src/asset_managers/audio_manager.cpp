#include "../../include/asset_manager/audio_manager.hpp"

#include <iostream>

#include <SDL2/SDL_mixer.h>

namespace Managers {

  AudioManager& AudioManager::instance() {
    static AudioManager instance;
    return instance;
  };

  Mix_Chunk* AudioManager::get_or_load(const std::string& path) {
    const std::string normalized_path = asset_path(path);
    auto              audio           = get_asset(path);
    if (audio)
      return audio;

    Mix_Chunk* chunk = Mix_LoadWAV(normalized_path.c_str());
    if (!chunk) {
      std::cerr << "Failed to load sound " << path << ": " << Mix_GetError() << "\n";
      return nullptr;
    }

    m_assets[normalized_path] = chunk;
    return chunk;
  }

  void AudioManager::init(int frequency, Uint16 format, int channels, int chunksize) {
    if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
      throw std::runtime_error("Failed to initialize SDL_mixer: " + std::string(Mix_GetError()));
    }
    std::cout << "[AudioManager] SDL_mixer initialized\n";
  }

  void AudioManager::play_sound(const GameAudioChannel& channel) {
    auto it = channels.find(channel);
    if (it == channels.end())
      return;

    const GameChannelSpecification& chan = it->second;

    Mix_Chunk* chunk = get_or_load(chan.path);
    if (chunk) {
      Mix_VolumeChunk(chunk, chan.volume);
      Mix_PlayChannel(static_cast<int>(chan.channel), chunk, chan.loop ? -1 : 0);
    }
  }

  void AudioManager::stop_channel(GameAudioChannel channel) {
    if (channel != GameAudioChannel::FREE_CHANNEL) {
      Mix_HaltChannel(static_cast<int>(channel));
    }
  }

  bool AudioManager::channel_playing(GameAudioChannel channel) {
    return Mix_Playing(static_cast<int>(channel)) != 0;
  }

} // namespace Managers