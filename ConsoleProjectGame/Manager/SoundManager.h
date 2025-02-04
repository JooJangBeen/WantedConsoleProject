#pragma once
#include <fmod.hpp>
#include <unordered_map>
#include <string>
#include<iostream>

#define MAX_SOUNDCOUNT 10
#define MAX_CHANNELCOUNT 20
#define MAX_VOLUME 100

using namespace FMOD;

enum class SoundType
{
    None = -1,
    BackGround = 0,
    EffectSound = 1,
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

public:
	static SoundManager& Get() { return *instance; }

    Sound* FPlaySound(std::string soundName, SoundType soundType, FMOD_MODE mode = FMOD_LOOP_OFF);
    void SetBackGroudVolume(float volume = 100.0f);
    void SetEffectVolume(float volume = 100.0f);

    Channel* GetChannel(SoundType soundType);

    void AllStop();





private:
	static SoundManager* instance;
    int soundCount = 0;
    std::string soundName = "";

    std::unordered_map<std::string, int> soundMap;
    System* system = nullptr;
    Sound** sound = nullptr;
    Channel** channel = nullptr;
    float volumeBG;
    float volumeEF;
    void* extradriverdata = nullptr;
};