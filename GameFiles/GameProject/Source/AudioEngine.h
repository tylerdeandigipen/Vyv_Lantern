#pragma once
/*!
@file    AudioEngine.h
@author	 Louis Wang

         Header file for AudioEngine.cpp, for sfx and bgm
*//*__________________________________________________________________________*/
/*                                                                   includes
----------------------------------------------------------------------------- */
#include <iostream>
#include <chrono>
#include <thread>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
/*                                                                   instances
----------------------------------------------------------------------------- */
#define AudioManager _audioManager::Instance()
/*                                                                   Class
----------------------------------------------------------------------------- */
class _audioManager
{

    /*                                                              Private Class
   ----------------------------------------------------------------------------- */
private:
    _audioManager(void);
    ~_audioManager(void);
    FMOD::System* fmodSystem = NULL;
    FMOD::Channel* SFXchannel = nullptr;
    FMOD::Channel* musicChannel = nullptr;
    FMOD::Channel* voiceChannel = nullptr;
    FMOD::ChannelGroup* channelGroup = nullptr;
    FMOD::ChannelGroup* musicChannelGroup = nullptr;
    std::map<std::string, FMOD::Sound*> soundDatabase;
    std::map<std::string, FMOD::Sound*> musicDatabase;
    std::map<std::string, FMOD::Sound*> voiceDatabase;
    bool isActive = true;
    json jsonData;

    /*                                                             Public Class
    ----------------------------------------------------------------------------- */
    public:
        static _audioManager& Instance(void);
        FMOD::Channel* GetMusicChannel(void);
        FMOD::Channel* GetAudioChannel(void);
        FMOD::Channel* GetVoiceChannel(void);
        void PlaySFX(std::string audiClip);
        void PlayMusic(std::string musicTrack);
        void PlayVoice(std::string voiceClip);
        void StopSFX(void);
        void StopMusic(void);
        void StopVoice();
        void SetAudioVolume(float volume);
        void SetMusicVolume(float volume);
        void SetVoiceVolume(float volume);
        void IncreaseMusicVolume();
        void IncreaseSFXVolume();
        void IncreaseVoiceVolume();
        void DecreaseMusicVolume();
        void DecreaseSFXVolume();
        void DecreaseVoiceVolume();
        void Update(void);
        void LoadMusicFromJSON(std::string jsonFilePath);
        void LoadSFXFromJSON(std::string jsonFilePath);
        void LoadVoiceFromJSON(std::string jsonFilePath);
        void LoadSFX(std::string musicName, std::string filePath);
        void LoadMusic(std::string musicName, std::string filePath);
        void LoadVoice(std::string name);
        void UnLoadSFX(std::string name);
        void UnloadMusic(std::string name);
        void UnloadVoice(std::string name);
        void Free(void);
        FMOD::Sound* GetSound(std::string name);
        FMOD::Sound* GetMusic(std::string name);
    };

/*                                                             Call back function
 ----------------------------------------------------------------------------- */
FMOD_RESULT F_CALLBACK channelGroupCallback(FMOD_CHANNELCONTROL* channelControl,
    FMOD_CHANNELCONTROL_TYPE controlType,
    FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
    void* commandData1,
    void* commandData2);