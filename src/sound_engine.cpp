#include "sound_engine.h"
#include "utils.h"

SoundEngine::SoundEngine()
    : music_filename("")
{
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
}

SoundEngine::~SoundEngine() {
    Mix_CloseAudio();
}

void SoundEngine::loadSound(Sound* sound, const std::string& filename) {
    if (!sound) return;

    Mix_Chunk* chunk = NULL;
    
    // check sound cache first
    chunk = cacheLookup(filename);
    if (chunk) {
        sound->filename = filename;
        sound->chunk = chunk;
        sound->init(this);
        return;
    }

    // load the sound from disk and cache it
    chunk = Mix_LoadWAV(filename.c_str());
    if (chunk) {
        sound->filename = filename;
        sound->chunk = chunk;
        cacheStore(sound, false);
    }
    sound->init(this);

    if (sound->filename == "")
        logError("SoundEngine: Could not load sound file: '%s'\n", filename.c_str());
}

void SoundEngine::loadMusic(Sound* sound, const std::string& filename) {
    if (!sound) return;

    Mix_Music* music = NULL;
    
    // check sound cache first
    music = cacheLookupMusic(filename);
    if (music) {
        sound->filename = filename;
        sound->music = music;
        sound->init(this);
        return;
    }

    // load the music from disk and cache it
    music = Mix_LoadMUS(filename.c_str());
    if (music) {
        sound->filename = filename;
        sound->music = music;
        cacheStore(sound, true);
    }
    sound->init(this);

    if (sound->filename == "")
        logError("SoundEngine: Could not load music file: '%s'\n", filename.c_str());
}

void SoundEngine::destroySound(Sound* sound) {
    if (sound)
        cacheRemove(sound->filename);
}

void SoundEngine::cacheStore(const Sound* sound, bool is_music) {
    if (sound->filename != "") {
        SoundCache snd;
        snd.filename = sound->filename;
        snd.ref = 1;

        if (!is_music && sound->chunk != NULL) {
            snd.chunk = sound->chunk;
            snd.music = NULL;
            cache.push_back(snd);
        }
        else if (is_music && sound->music != NULL) {
            snd.music = sound->music;
            snd.chunk = NULL;
            cache.push_back(snd);
        }
    }
}

void SoundEngine::cacheRemove(const std::string& filename) {
    for (unsigned i=0; i<cache.size(); ++i) {
        if (filename == cache[i].filename) {
            cache[i].ref--;
            if (cache[i].ref <= 0) {
                if (cache[i].chunk)
                    Mix_FreeChunk(cache[i].chunk);
                if (cache[i].music)
                    Mix_FreeMusic(cache[i].music);
                cache.erase(cache.begin()+i);
                return;
            }
        }
    }
}

Mix_Chunk* SoundEngine::cacheLookup(const std::string& filename) {
    for (unsigned i=0; i<cache.size(); ++i) {
        if (!cache[i].music && filename == cache[i].filename) {
            cache[i].ref++;
            return cache[i].chunk;
        }
    }

    return NULL;
}

Mix_Music* SoundEngine::cacheLookupMusic(const std::string& filename) {
    for (unsigned i=0; i<cache.size(); ++i) {
        if (cache[i].music && filename == cache[i].filename) {
            cache[i].ref++;
            return cache[i].music;
        }
    }

    return NULL;
}

void SoundEngine::playMusic(const Sound* sound) {
    if (sound && sound->music && sound->filename != music_filename) {
        Mix_HaltMusic();
        Mix_PlayMusic(sound->music, -1);
    }
}


// Sound class

Sound::Sound()
    : filename("")
    , chunk(NULL)
    , music(NULL)
    , sound_engine(NULL)
{
}

Sound::~Sound() {
    if (sound_engine)
        sound_engine->destroySound(this);
}

void Sound::init(SoundEngine* _sound_engine) {
    sound_engine = _sound_engine;
}

void Sound::load(SoundEngine* _sound_engine, const std::string _filename, bool is_music) {
    if (_sound_engine) {
        if (is_music)
            _sound_engine->loadMusic(this, _filename);
        else
            _sound_engine->loadSound(this, _filename);
    }
}

void Sound::play() {
    if (chunk)
        Mix_PlayChannel(-1, chunk, 0);
    else if (sound_engine && music)
        sound_engine->playMusic(this);
}

void Sound::stop() {
    if (music)
        Mix_HaltMusic();
}
