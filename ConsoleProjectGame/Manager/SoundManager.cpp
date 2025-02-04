#include "SoundManager.h"

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager()
{
    instance = this;

    System_Create(&system);
    sound = new Sound * [MAX_SOUNDCOUNT];
    channel = new Channel * [MAX_CHANNELCOUNT];
    volumeBG = 60;
    volumeEF = 60;

    // DSP 버퍼 크기 설정
    unsigned int dspBufferLength = 256; // 샘플 크기
    int dspBufferCount = 4;             // 버퍼 개수
    system->setDSPBufferSize(dspBufferLength, dspBufferCount);

    // FMOD 시스템 초기화
    system->init(MAX_CHANNELCOUNT, FMOD_INIT_NORMAL, extradriverdata);


    //system->createSound("../Assets/Music/Onlyforyou.mp3", FMOD_LOOP_OFF, 0, &sound[0]);
    //system->createSound("../Assets/Music/Magnolia.mp3", FMOD_LOOP_OFF, 0, &sound[1]);
    //system->createSound("../Assets/Music/EndoftheMoonlight.mp3", FMOD_LOOP_OFF, 0, &sound[2]);
    //soundCount = 3;
}

SoundManager::~SoundManager()
{
    if (channel != nullptr)
    {
        for (int i = 0; i < MAX_CHANNELCOUNT; i++)
        {
            if (channel[i])
                channel[i]->stop();
        }
    }
    if (sound != nullptr)
    {
        for (int i = 0; i < MAX_SOUNDCOUNT; i++)
        {
            if (sound[i])
                sound[i]->release();
        }
    }

    delete channel;
    delete sound;

    if (system != nullptr)
    {
        system->release();
        system->close();
    }
}

Sound* SoundManager::FPlaySound(std::string soundName, SoundType soundType, FMOD_MODE mode)
{
    bool isPlaying = false;
    int channelCount = (int)soundType;
    int tempSoundCount = 0;

    if (soundCount >= MAX_SOUNDCOUNT)
    {
        __debugbreak();
        return nullptr;
    }

    this->soundName = soundName;


    if (soundMap.empty() || !(soundMap.find(soundName) != soundMap.end()))
    {
        tempSoundCount = soundCount;
        std::string fullPath = "../Assets/Music/" + soundName + ".mp3";
        system->createSound(fullPath.c_str(), mode, 0, &sound[soundCount]);
        soundMap.insert({ soundName, soundCount });


        channel[channelCount]->isPlaying(&isPlaying);
        if (isPlaying)
        {
            channel[channelCount]->stop();
        }

        channel[channelCount]->setVolume(volumeBG * 0.01f);
        system->playSound(sound[soundCount],nullptr, false, &channel[channelCount]);
        ++soundCount;
    }
    else
    {
        tempSoundCount = soundMap[soundName];

        if (soundType == SoundType::EffectSound)
        {
            // 배경 사운드 채널 가져오기
            if (channel[(int)SoundType::BackGround] == nullptr) return nullptr; // 배경 사운드가 없으면 반환

            // 배경 사운드의 현재 DSPClock 가져오기
            unsigned long long bgDSPClock = 0;
            channel[(int)SoundType::BackGround]->getDSPClock(&bgDSPClock, nullptr);


            // 타격음 재생 시간 조정
            unsigned long long dspclock_start = bgDSPClock + 256; // 256 샘플 앞당겨 재생
            unsigned long long dspclock_end = 0; // 종료 시간 설정 안 함

            // 타격음 재생
            for (int i = 1; i < MAX_CHANNELCOUNT; ++i)
            {
                channel[i]->isPlaying(&isPlaying);

                if (isPlaying) continue;

                system->playSound(sound[tempSoundCount], nullptr, true, &channel[i]);
                channel[i]->setDelay(dspclock_start, dspclock_end, false); // 딜레이 설정
                channel[i]->setVolume(volumeEF * 0.01f); // 볼륨 설정
                channel[i]->setPaused(false); // 재생 시작

                break;
            }
        }
        else
        {
            channel[channelCount]->isPlaying(&isPlaying);
            if (isPlaying)
            {
                channel[channelCount]->stop();
            }
            system->playSound(sound[tempSoundCount], nullptr, false, &channel[channelCount]);
        }
    }

    if (soundType == SoundType::BackGround)
    {
        return sound[tempSoundCount];
    }
    else
    {
        return nullptr;
    }

}

void SoundManager::SetBackGroudVolume(float volume)
{
    if (volume > 100.0f) volumeBG = 100.f;
    else if (volume < 0.0f) volumeBG = 0.0f;

    volumeBG = volume;

    if (channel[(int)SoundType::BackGround] != nullptr)
    {
        channel[(int)SoundType::BackGround]->setVolume(volumeBG * 0.01f);
    }
}

void SoundManager::SetEffectVolume(float volume)
{
    if (volume > 100.0f) volumeEF = 100.f;
    else if (volume < 0.0f) volumeEF = 0.0f;

    volumeEF = volume;

    if (channel[(int)SoundType::EffectSound] != nullptr)
    {
        channel[(int)SoundType::EffectSound]->setVolume(volumeEF);
    }
}

Channel* SoundManager::GetChannel(SoundType soundType)
{
    if (channel[(int)soundType] != nullptr)
    {
        return channel[(int)soundType];
    }
    
    return nullptr;
}

void SoundManager::AllStop()
{
    for (int i = 0; i < MAX_CHANNELCOUNT; ++i)
    {
        channel[i]->stop();
    }
}
