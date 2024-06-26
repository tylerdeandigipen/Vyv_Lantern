//------------------------------------------------------------------------------
//
// File Name:	AudioEngine.cpp
// Author(s):	Louis Wang
// Purpose:		Controlling of the audio using FMOD.
//
// Copyright  � 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "AudioEngine.h"
#include "Logging.h"
#include <random>

std::random_device random;
std::mt19937 generator(random());
std::uniform_int_distribution<int> distributor(0, 2);
const int MAX_MUSIC_TRACKS = 10;
FMOD::Channel* musicChannels[MAX_MUSIC_TRACKS]; // Define an array of music channels

/*
*   Instances
----------------------------------------------------------------------------- */
_audioManager& _audioManager::Instance(void)
{
	static _audioManager instance;
	return instance;
}
/*                                                                  Constructor
----------------------------------------------------------------------------- */
_audioManager::_audioManager(void)
{
	/*
	* Create the main system object.
	----------------------------------------------------------------------------- */
	FMOD::System_Create(&fmodSystem);

	/*
   *   Initialize FMOD.
   ----------------------------------------------------------------------------- */
	fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

	/*
	*   Create the channel group
	----------------------------------------------------------------------------- */
	fmodSystem->createChannelGroup("inGameSoundEffects", &channelGroup);
	fmodSystem->createChannelGroup("inGameSoundEffects", &SFXChannelGroup);
	fmodSystem->createChannelGroup("inGameSoundEffects", &musicChannelGroup);
	fmodSystem->createChannelGroup("inGameSoundEffects", &voiceChannelGroup);//this one i don't think is going to be used

	/*
	*   Set music channel group
	----------------------------------------------------------------------------- */

	//This logically doesn't work
	//SFXchannel->setChannelGroup(channelGroup);
	//musicChannel->setChannelGroup(channelGroup);
	//voiceChannel->setChannelGroup(channelGroup);

	/*
	*   Set a callback on the channel.
	----------------------------------------------------------------------------- */

	//musicChannel->setCallback(&channelGroupCallback);

	for (int i = 0; i < MAX_MUSIC_TRACKS; i++) {
		fmodSystem->playSound(nullptr, musicChannelGroup, true, &musicChannels[i]); // Create music channels
	}

	/*New channel for Musics, like bgm and stuff that plays parallel*/
	fmodSystem->createChannelGroup("Music", &musicChannelGroup);
}

/*                                                                  Destructor
----------------------------------------------------------------------------- */
_audioManager::~_audioManager(void)
{
}

/*!				void _audioManager::PlaySFX(string clipName)
@param			string clipName
@return none

				Plays music clip
*/
void _audioManager::PlaySFX(std::string clipName, float volume)
{
	// Create the sound.
	FMOD::Sound* sound = GetSound(clipName);
	if (sound)
	{
		FMOD::Channel* channel;
		fmodSystem->playSound(sound, SFXChannelGroup, false, &channel);
		channel->setVolume(volume);
	}
}

/*!				void _audioManager::PlayMusic(string musicTrack)
@param			string musicTrack
@return none

				Plays music track
*/
void _audioManager::PlayMusic(std::string musicTrack)
{
	// Create the sound.
	FMOD::Sound* audioClip = nullptr;
	auto search = musicDatabase.find(musicTrack);
	if (search != musicDatabase.end())
		audioClip = search->second;

	// Check if the audioClip is valid.
	if (audioClip) {
		fmodSystem->playSound(audioClip, musicChannelGroup, false, nullptr);

		// Handle channel-specific settings (fade, etc.) here if needed.
	}
}

/*!				void _audioManager::PlayVoice(string voiceClip)
@param			string voiceClip
@return none

				Plays voice
*/
void _audioManager::PlayVoice(std::string voiceClip)
{
	// Create the sound.
	FMOD::Sound* audioClip = 0;
	auto search = voiceDatabase.find(voiceClip);
	if (search != voiceDatabase.end())
		audioClip = search->second;

	// Play the sound.
	fmodSystem->playSound(audioClip, voiceChannelGroup, false, nullptr);
}

/*!				void _audioManager::StopMusic(void)
@param			void
@return none

				Stop playing music
*/
void _audioManager::StopMusic(void)
{
	// Kill sound
	//unsigned long long dspclock;
	//FMOD::Sound* snd;
	//int rate;
	musicChannelGroup->stop();

	//musicChannel->getDSPClock(0, &dspclock);
	//fmodSystem->getSoftwareFormat(&rate, 0, 0);
	//musicChannel->addFadePoint(dspclock, 1.0f);
	//musicChannel->addFadePoint(dspclock + rate, 0.0f);
	//musicChannel->setDelay(0, dspclock + rate, true);
	//musicChannel->setPaused(true);
}

/*!				void _audioManager::StopSFX(void)
@param			void
@return none

				Stop playing Sound Effects
*/
void _audioManager::StopSFX(void)
{
	// stop play the sound.
	SFXChannelGroup->stop();
}

/*!				void _audioManager::StopVoice(void)
@param			void
@return none

				Stop playing Voices
*/
void _audioManager::StopVoice(void)
{
	// stop play the sound.
	voiceChannelGroup->stop();
}

void _audioManager::ResumeMusic(void)
{
	musicChannelGroup->setPaused(false);
}

void _audioManager::ResumeSFX(void)
{
	SFXChannelGroup->setPaused(false);
}

void _audioManager::PauseMusic(void)
{
	musicChannelGroup->setPaused(true);
}

void _audioManager::PauseSFX(void)
{
	SFXChannelGroup->setPaused(true);
}

/*!				void _audioManager::SetAudioVolume(float volume)
@param			float volume
@return none

				Set volume for audios
*/
void _audioManager::SetAudioVolume(float volume)
{
	SFXChannelGroup->setPaused(true);
	SFXChannelGroup->setVolume(volume);
	SFXChannelGroup->setPaused(false);
}

/*!				void _audioManager::SetMusicVolume(float volume)
@param			float volume
@return none

				Set volume for music channel
*/
void _audioManager::SetMusicVolume(float volume)
{
	musicChannelGroup->setPaused(true);
	musicChannelGroup->setVolume(volume);
	musicChannelGroup->setPaused(false);
}

void _audioManager::SetVoiceVolume(float volume)
{
	voiceChannelGroup->setPaused(true);
	voiceChannelGroup->setVolume(volume);
	voiceChannelGroup->setPaused(false);
}

void _audioManager::IncreaseMusicVolume(float volume)
{
	float currentVolume;
	musicChannelGroup->getVolume(&currentVolume);

	float newVolume = currentVolume + volume;
	if (newVolume < 0.0f) {
		newVolume = 0.0f;
	}

	musicChannelGroup->setVolume(newVolume);
}

void _audioManager::IncreaseSFXVolume(float volume)
{
	float currentVolume;
	SFXChannelGroup->getVolume(&currentVolume);

	float newVolume = currentVolume + volume;
	if (newVolume < 0.0f) {
		newVolume = 0.0f;
	}

	SFXChannelGroup->setVolume(newVolume);
}

void _audioManager::IncreaseVoiceVolume(float volume)
{
	float currentVolume;
	voiceChannelGroup->getVolume(&currentVolume);

	float newVolume = currentVolume + volume;
	if (newVolume < 0.0f) {
		newVolume = 0.0f;
	}

	voiceChannelGroup->setVolume(newVolume);
}

void _audioManager::DecreaseMusicVolume(float volume)
{
	float currentVolume;
	musicChannelGroup->getVolume(&currentVolume);

	float newVolume = currentVolume - volume;
	if (newVolume < 0.0f) {
		newVolume = 0.0f;
	}

	musicChannelGroup->setVolume(newVolume);
}

void _audioManager::DecreaseSFXVolume(float volume)
{
	float currentVolume;
	SFXChannelGroup->getVolume(&currentVolume);

	float newVolume = currentVolume - volume;
	if (newVolume < 0.0f) {
		newVolume = 0.0f;
	}

	SFXChannelGroup->setVolume(newVolume);
}

void _audioManager::DecreaseVoiceVolume(float volume)
{
	float currentVolume;
	voiceChannelGroup->getVolume(&currentVolume);

	float newVolume = currentVolume - volume;
	if (newVolume < 0.0f) {
		newVolume = 0.0f;
	}

	voiceChannelGroup->setVolume(newVolume);
}

/*!				void _audioManager::Update()
@param
@return none

			   update music play
*/
void _audioManager::Update()
{
	fmodSystem->update();
}

void _audioManager::LoadMusicFromJSON(std::string jsonFilePath)
{
	// load in the file
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		Logging::GetInstance("debugLog.log").LogLine("Could not open %s", jsonFilePath.c_str());

		//perror("JSON file does not exist");
		return;
	}
	/*Parse file contents into this createion*/
	file >> jsonData;
	Logging::GetInstance("debugLog.log").LogLine("Opened %s", jsonFilePath.c_str());
	for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
		const std::string musicName = it.key();
		const std::string filePath = it.value();

		// Now you can use the filePath to load the music
		LoadMusic(musicName, filePath);
	}
}

void _audioManager::LoadSFXFromJSON(std::string jsonFilePath)
{
	// load in the file
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		Logging::GetInstance("debugLog.log").LogLine("Could not open %s", jsonFilePath.c_str());

		//perror("JSON file does not exist");
		return;
	}
	/*Parse file contents into this beatiful cration*/
	file >> jsonData;
	Logging::GetInstance("debugLog.log").LogLine("Opened %s", jsonFilePath.c_str());
	for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
		const std::string musicName = it.key();
		const std::string filePath = it.value();

		// Now you can use the filePath to load the music
		LoadSFX(musicName, filePath);
	}
}

/*For next semester if needed*/
void _audioManager::LoadVoiceFromJSON(std::string jsonFilePath)
{
}

/*!				void _audioManager::LoadSFX(string name)
{
@param          string name
@return none

			   Load sound into directory
*/
void _audioManager::LoadSFX(std::string musicName, std::string filePath)
{
	if (soundDatabase.find(musicName) == soundDatabase.end())
	{
		char* pathName = new char[filePath.length() + 1];
		std::copy(filePath.begin(), filePath.end(), pathName);

		// Add null character at the end to terminate the string
		pathName[filePath.length()] = '\0'; // Ensure proper termination
		FMOD::Sound* sound = 0;
		fmodSystem->createSound(pathName, FMOD_DEFAULT, nullptr, &sound);
		soundDatabase[musicName] = sound;
		delete[] pathName;
	}
}

/*!				void _audioManager::LoadVoice(string name)
{
@param          string name
@return none

			   Load voice into directory
*/
void _audioManager::LoadVoice(std::string name)
{
	if (voiceDatabase.find(name) == voiceDatabase.end())
	{
		std::string pathString = "Assets/Audio/Voice-over/" + name + '\0';
		char* pathName = new char[pathString.length() + 1];
		std::copy(pathString.begin(), pathString.end(), pathName);
		FMOD::Sound* sound = 0;
		fmodSystem->createSound(pathName, FMOD_DEFAULT, nullptr, &sound);
		voiceDatabase[name] = sound;
		delete[] pathName;
	}
}

/*!				void _audioManager::LoadMusic(std::string musicName, std::string filePath)
{
@param          std::string musicName, std::string filePath
@return none

			   Load music into directory
*/
void _audioManager::LoadMusic(std::string musicName, std::string filePath)
{
	if (musicDatabase.find(musicName) == musicDatabase.end())
	{
		char* pathName = new char[filePath.length() + 1];
		std::copy(filePath.begin(), filePath.end(), pathName);

		// Add null character at the end to terminate the string
		pathName[filePath.length()] = '\0'; // Ensure proper termination
		FMOD::Sound* music = 0;
		fmodSystem->createSound(pathName, FMOD_LOOP_NORMAL, nullptr, &music);
		musicDatabase[musicName] = music;
		delete[] pathName;
	}
}

/*!				void _audioManager::UnLoadSFX(string name)
{
@param          string name
@return none

			   Unload sound into directory
*/
void _audioManager::UnLoadSFX(std::string name)
{
	if (isActive && soundDatabase.find(name) != soundDatabase.end())
		soundDatabase[name]->release();
}

/*!				void _audioManager::UnloadMusic(string name)
{
@param          string name
@return none

			   Unload music into directory
*/
void _audioManager::UnloadMusic(std::string name)
{
	if (isActive && musicDatabase.find(name) != musicDatabase.end())
		musicDatabase[name]->release();
}

/*!				void _audioManager::UnloadVoice(string name)
{
@param          string name
@return none

			   Unload voice into directory
*/
void _audioManager::UnloadVoice(std::string name)
{
	if (isActive && voiceDatabase.find(name) != voiceDatabase.end())
		voiceDatabase[name]->release();
}

void _audioManager::UnloadAll()
{
	for (auto& sfxSounds : soundDatabase)
	{
		UnLoadSFX(sfxSounds.first);
		sfxSounds.second = nullptr;
	}

	for (auto& musicSounds : musicDatabase)
	{
		UnLoadSFX(musicSounds.first);
		musicSounds.second = nullptr;
	}
}

/*!				void _audioManager::Free(void)
{
@param
@return none

			   Free memory space
*/
void _audioManager::Free(void)
{
	UnloadAll();
	fmodSystem->release();
	isActive = false;
}

/*!				void _audioManager::GetSound(string name)
{
@param          string name
@return none

			   get sound
*/
FMOD::Sound* _audioManager::GetSound(std::string name)
{
	auto search = soundDatabase.find(name);
	if (search != soundDatabase.end())
		return search->second;
	return NULL;
}

FMOD::Sound* _audioManager::GetMusic(std::string name)
{
	auto search = musicDatabase.find(name);
	if (search != musicDatabase.end())
		return search->second;
	return NULL;
}

/*
   *   get channel
   ----------------------------------------------------------------------------- */
FMOD::ChannelGroup* _audioManager::GetMusicChannel(void)
{
	return musicChannelGroup;
}

FMOD::ChannelGroup* _audioManager::GetAudioChannel()
{
	return SFXChannelGroup;
}

FMOD::ChannelGroup* _audioManager::GetVoiceChannel()
{
	return voiceChannelGroup;
}

//FMOD_RESULT F_CALLBACK channelGroupCallback(FMOD_CHANNELCONTROL* channelControl,
//    FMOD_CHANNELCONTROL_TYPE controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
//    void* commandData1, void* commandData2)
//{
//    if (controlType == FMOD_CHANNELCONTROL_CHANNEL
//        && AudioManager.GetMusicChannel() == (FMOD::Channel*)channelControl
//        && callbackType == FMOD_CHANNELCONTROL_CALLBACK_END)
//    {
//        // music ended
//    }

//    commandData1;
//    commandData2;

//    return FMOD_OK;
//}