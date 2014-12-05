#pragma once

#include "common.h"

class Sound;

typedef struct {
    std::string filename;
    Mix_Chunk* chunk;
    Mix_Music* music;
    int ref;
} SoundCache;

class SoundEngine {
public:
    SoundEngine();
    ~SoundEngine();
    void loadSound(Sound* sound, const std::string& filename);
    void loadMusic(Sound* sound, const std::string& filename);
    void destroySound(Sound* sound);
    void playMusic(const Sound* sound);

private:
    void cacheStore(const Sound* sound, bool is_music);
    void cacheRemove(const std::string& filename);
    Mix_Chunk* cacheLookup(const std::string& filename);
    Mix_Music* cacheLookupMusic(const std::string& filename);

    std::vector<SoundCache> cache;
    std::string music_filename;
};

class Sound {
public:
    Sound();
    ~Sound();
    void init(SoundEngine* _sound_engine);
    void load(SoundEngine* _sound_engine, const std::string _filename, bool is_music = false);
    void play();
    void stop(); // music only

    std::string filename;
    Mix_Chunk* chunk;
    Mix_Music* music;

private:
    SoundEngine* sound_engine;
};

