// ==============================================================
// XRSound engine implementation.
// 
// Copyright (c) 2018-2021 Douglas Beachy
// Licensed under the MIT License
// ==============================================================

#include "XRSoundEngine.h"

// Functionality added in XRSound version 3.0

bool XRSoundEngine::SetPan(const int soundID, const float pan)
{
    if (!IsKlangEngineInitialized() || (pan < -1.0) || (pan > 1.0))
        return false;

    bool retVal = false;
    WavContext *pContext = FindWavContext(soundID);
    if (pContext)
    {
        ISound *pISound = pContext->pISound;
        if (pISound)   // was sound ever started via PlayWav?
        {
            // irrKlang has pan direction inverted with SpaceXpanse's X coordinate system, so flip it
            pISound->setPan(-pan);
            retVal = true;
        }
    }
    return retVal;
}

float XRSoundEngine::GetPan(const int soundID)
{
    if (!IsKlangEngineInitialized())
        return -100;

    float retVal = -100;
    WavContext *pContext = FindWavContext(soundID);
    if (pContext)
    {
        ISound *pISound = pContext->pISound;
        if (pISound)   // was sound ever started via PlayWav?
        {
            // irrKlang has pan direction inverted with SpaceXpanse's X coordinate system, so flip it
            retVal = -(pISound->getPan());
        }
    }
    return retVal;
}

bool XRSoundEngine::SetPlaybackSpeed(const int soundID, const float speed)
{
    if (!IsKlangEngineInitialized())
        return false;

    bool retVal = false;
    WavContext* pContext = FindWavContext(soundID);
    if (pContext)
    {
        ISound *pISound = pContext->pISound;
        if (pISound)   // was sound ever started via PlayWav?
            retVal = pISound->setPlaybackSpeed(speed);
    }
    return retVal;
}

float XRSoundEngine::GetPlaybackSpeed(const int soundID)
{
    if (!IsKlangEngineInitialized())
        return 0;

    float retVal = 0;
    WavContext *pContext = FindWavContext(soundID);
    if (pContext)
    {
        ISound* pISound = pContext->pISound;
        if (pISound)   // was sound ever started via PlayWav?
            retVal = pISound->getPlaybackSpeed();
    }
    return retVal;
}


bool XRSoundEngine::SetPlayPosition(const int soundID, const unsigned int positionMillis)
{
    if (!IsKlangEngineInitialized())
        return false;

    bool retVal = false;
    WavContext* pContext = FindWavContext(soundID);
    if (pContext)
    {
        ISound *pISound = pContext->pISound;
        if (pISound)   // was sound ever started via PlayWav?
            retVal = pISound->setPlayPosition(positionMillis);
    }
    return retVal;
}

int XRSoundEngine::GetPlayPosition(const int soundID)
{
    if (!IsKlangEngineInitialized())
        return -1;

    int retVal = -1;
    WavContext *pContext = FindWavContext(soundID);
    if (pContext)
    {
        ISound* pISound = pContext->pISound;
        if (pISound)   // was sound ever started via PlayWav?
        {
            // 2 millions seconds is 555 hours, so casting to a signed integer is (quite) sufficent here
            retVal = static_cast<int>(pISound->getPlayPosition());
        }
    }
    return retVal;
}
