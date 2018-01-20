/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AudioPlayer.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

AudioPlayer::AudioPlayer()
{
}

AudioPlayer::~AudioPlayer()
{
}

uint32_t AudioPlayer::GetChannelCount() const
{
    if (m_IsSound)
    {
        return m_SoundBuffer.getChannelCount();
    }
    else
    {
        return m_Music.getChannelCount();
    }
}

uint32_t AudioPlayer::GetSampleRate() const
{
    if (m_IsSound)
    {
        return m_SoundBuffer.getSampleRate();
    }
    else
    {
        return m_Music.getSampleRate();
    }
}

float AudioPlayer::GetDuration() const
{
    if (m_IsSound)
    {
        return m_SoundBuffer.getDuration().asSeconds();
    }
    else
    {
        return m_Music.getDuration().asSeconds();
    }
}

void AudioPlayer::SetPosition(const glm::vec3& pos)
{
    if (m_IsSound)
    {
        m_Sound.setPosition(pos.x, pos.y, pos.z);
    }
    else
    {
        m_Music.setPosition(pos.x, pos.y, pos.z);
    }

    m_AudioPlayerState.m_Position = pos;
}

void AudioPlayer::SetVolume(float volume)
{
    volume = glm::clamp(volume, 0.0f, 100.0f);

    if (m_IsSound)
    {
        m_Sound.setVolume(volume);
    }
    else
    {
        m_Music.setVolume(volume);
    }

    m_AudioPlayerState.m_Volume = volume;
}

void AudioPlayer::SetAttenuation(float attenuation)
{
    attenuation = (attenuation < 0.0f) ? 0.0f : attenuation;

    if (m_IsSound)
    {
        m_Sound.setAttenuation(attenuation);
    }
    else
    {
        m_Music.setAttenuation(attenuation);
    }

    m_AudioPlayerState.m_Attenuation = attenuation;
}

void AudioPlayer::SetMinDistance(float minDistance)
{
    minDistance = (minDistance < 1.0f) ? 1.0f : minDistance;

    if (m_IsSound)
    {
        m_Sound.setMinDistance(minDistance);
    }
    else
    {
        m_Music.setMinDistance(minDistance);
    }

    m_AudioPlayerState.m_MinDistance = minDistance;
}

void AudioPlayer::SetPitch(float pitch)
{
    if (m_IsSound)
    {
        m_Sound.setPitch(pitch);
    }
    else
    {
        m_Music.setPitch(pitch);
    }

    m_AudioPlayerState.m_Pitch = pitch;
}

void AudioPlayer::SetLoop(bool looped)
{
    if (m_IsSound)
    {
        m_Sound.setLoop(looped);
    }
    else
    {
        m_Music.setLoop(looped);
    }

    m_AudioPlayerState.m_Loop = looped;
}

void AudioPlayer::Set3D(bool is3d)
{
    if (m_IsSound)
    {
        m_Sound.setRelativeToListener(is3d);
    }
    else
    {
        m_Music.setRelativeToListener(is3d);
    }

    m_AudioPlayerState.m_Is3D = is3d;
}

AEResult AudioPlayer::Initialize(const uint8_t* data, uint32_t size)
{
    /////////////////////////////////////////////////
    //Pre-checks
    AEAssert(data != nullptr);
    if (data == nullptr)
    {
        return AEResult::NullParameter;
    }

    AEAssert(size != 0);
    if (size == 0)
    {
        return AEResult::ZeroSize;
    }

    m_IsReady = false;

    /////////////////////////////////////////////////
    //Init Audio
    if (m_IsSound)
    {
        m_Sound.stop();

        AETODO("Check return");
        m_SoundBuffer.loadFromMemory((const void*)data, size);

        m_Sound.setBuffer(m_SoundBuffer);

        m_Sound.setVolume(m_AudioPlayerState.m_Volume);
        m_Sound.setMinDistance(m_AudioPlayerState.m_MinDistance);
        m_Sound.setPitch(m_AudioPlayerState.m_Pitch);
        m_Sound.setAttenuation(m_AudioPlayerState.m_Attenuation);

        m_Sound.setPosition(m_AudioPlayerState.m_Position.x, m_AudioPlayerState.m_Position.y, m_AudioPlayerState.m_Position.z);

        m_Sound.setRelativeToListener(m_AudioPlayerState.m_Is3D);
        m_Sound.setLoop(m_AudioPlayerState.m_Loop);
    }
    else
    {
        m_Music.stop();

        AETODO("Check return");
        m_Music.openFromMemory((const void*)data, size);

        m_Music.setVolume(m_AudioPlayerState.m_Volume);
        m_Music.setMinDistance(m_AudioPlayerState.m_MinDistance);
        m_Music.setPitch(m_AudioPlayerState.m_Pitch);
        m_Music.setAttenuation(m_AudioPlayerState.m_Attenuation);

        m_Music.setPosition(m_AudioPlayerState.m_Position.x, m_AudioPlayerState.m_Position.y, m_AudioPlayerState.m_Position.z);

        m_Music.setRelativeToListener(m_AudioPlayerState.m_Is3D);
        m_Music.setLoop(m_AudioPlayerState.m_Loop);
    }

    m_AudioData = data;
    m_AudioDataSize = size;

    m_IsReady = true;

    return AEResult::Ok;
}

void AudioPlayer::StopAndUninitialize()
{
    m_IsReady = false;

    /////////////////////////////////////////////////
    //Stop Audio Data
    if (m_IsSound)
    {
        m_Sound.stop();
    }
    else
    {
        m_Music.stop();
    }
}

AEResult AudioPlayer::SetAsSoundOrMusicProcessing(bool asSound)
{
    if (m_IsSound == asSound)
    {
        return AEResult::Ok;
    }

    StopAndUninitialize();

    m_IsSound = asSound;

    return Initialize(m_AudioData, m_AudioDataSize);
}

AEResult AudioPlayer::Play()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_IsSound)
    {
        m_Sound.play();
    }
    else
    {
        m_Music.play();
    }

    return AEResult::Ok;
}

AEResult AudioPlayer::Stop()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_IsSound)
    {
        m_Sound.pause();
    }
    else
    {
        m_Music.pause();
    }

    return AEResult::Ok;
}

AEResult AudioPlayer::Pause()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_IsSound)
    {
        m_Sound.pause();
    }
    else
    {
        m_Music.pause();
    }

    return AEResult::Ok;
}
