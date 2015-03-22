/*
Copyright © 2014-2015 Justin Jacobs

This file is part of Noman's Dungeon.

Noman's Dungeon is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Noman's Dungeon is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Noman's Dungeon.  If not, see http://www.gnu.org/licenses/
*/


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

