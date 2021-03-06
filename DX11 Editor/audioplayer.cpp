// Library Includes
#include <iostream>
#include <stdio.h>
#include <string>

// Local Includes

// This Include
#include "audioplayer.h"

// Static Variables
AudioPlayer* AudioPlayer::s_pAudio = 0;

// Static Function Prototypes

// Implementation

/**
*
* AudioPlayer class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
AudioPlayer::AudioPlayer()
: m_pFMod( nullptr )
, m_pChannel( nullptr )
, m_pSound( nullptr )
, m_bSoundsAreLoaded(false)
{
	
}
/**
*
* AudioPlayer class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
AudioPlayer::~AudioPlayer()
{
	if (m_bSoundsAreLoaded)
	{
		if (m_pFMod)
		{
			for (int iSound = 0; iSound < SOUND_MAX; ++iSound)
			{
				m_pSound[iSound]->release();
			}
			delete[] m_pSound;
			m_pSound = 0;
		}
		m_pFMod->close();
		m_pFMod->release();
	}
}

/**
*
* AudioPlayer class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns success of initialisation
*
*/
bool 
AudioPlayer::Initialise(bool _bLoadSounds)
{
	if (_bLoadSounds)
	{
		//Initialise FMOD system
		FMOD_CAPS tFModCaps;
		FMOD_SPEAKERMODE tSpeakerMode;

		FMOD::System_Create(&m_pFMod);
		m_pFMod->getDriverCaps(0, &tFModCaps, 0, &tSpeakerMode);
		m_pFMod->setSpeakerMode(tSpeakerMode);

		FMOD_RESULT result = m_pFMod->init(100, FMOD_INIT_3D_RIGHTHANDED, 0);
		m_pFMod->set3DSettings(10.0f, 10.0f, 10.0f);
	
		printf("============== Loading Audio Files ==============\n");
		LoadSounds();
		printf("=================================================");
	}
	return true;
}
void 
AudioPlayer::LoadSounds()
{
	m_bSoundsAreLoaded = true;
	m_pSound = new FMOD::Sound*[SOUND_MAX];
	std::string* pFilenames = new std::string[SOUND_MAX];
	pFilenames[SOUND_BIRDCHIRP]		= "Assets/SoundFiles/birdChirp.wav";
	pFilenames[SOUND_FOOTSTEPS]		= "Assets/SoundFiles/footStep.wav";
	pFilenames[SOUND_BUTTONPRESS]	= "Assets/SoundFiles/buttonClick.wav";

	for(int iSound = 0; iSound < SOUND_MAX; ++iSound)
	{
		printf("\tLoading %s\n", pFilenames[iSound].c_str());
		m_pFMod->createSound(pFilenames[iSound].c_str(), FMOD_3D, 0, &m_pSound[iSound]);
		m_pSound[iSound]->set3DMinMaxDistance(1.0f, 100.0f);
		m_pSound[iSound]->setDefaults(44100.0f, 1.0f, 0.0f, 0);
	}

	delete[] pFilenames;
}
void 
AudioPlayer::Process()
{
	m_pFMod->update();
}

void 
AudioPlayer::Play3DSound(E3DSoundType _eSound, Math::Vector3& _vecPosition)
{
	if (m_bSoundsAreLoaded)
	{
		FMOD::Channel* pNewAudioChannel = 0;
		m_pFMod->playSound(FMOD_CHANNEL_FREE, m_pSound[_eSound], false, &pNewAudioChannel);//m_pFMod->playSound(FMOD_CHANNEL_REUSE, m_pSound, false, &m_pChannel);

		FMOD_VECTOR fmodvec3DeathPosition;
		fmodvec3DeathPosition.x = _vecPosition.x;
		fmodvec3DeathPosition.y = _vecPosition.y;
		fmodvec3DeathPosition.z = _vecPosition.z;

		m_pChannel->set3DAttributes(&fmodvec3DeathPosition, NULL);
		m_pChannel->setPaused(false);
	}
}
void 
AudioPlayer::SetListenerPosition(Math::Vector3& _rVecListenerPos, Math::Vector3& _rVecForward, Math::Vector3& _rVecUp)
{
	if (m_bSoundsAreLoaded)
	{
		FMOD_VECTOR vecListenerPos;
		vecListenerPos.x = _rVecListenerPos.x;
		vecListenerPos.y = _rVecListenerPos.y;
		vecListenerPos.z = _rVecListenerPos.z;

		FMOD_VECTOR vecVelocity;
		vecVelocity.x = _rVecForward.x;
		vecVelocity.y = _rVecForward.y;
		vecVelocity.z = _rVecForward.z;

		FMOD_VECTOR vecForward;
		vecForward.x = _rVecForward.x;
		vecForward.y = _rVecForward.y;
		vecForward.z = _rVecForward.z;

		FMOD_VECTOR vecUp;
		vecUp.x = _rVecUp.x;
		vecUp.y = _rVecUp.y;
		vecUp.z = _rVecUp.z;

		m_pFMod->set3DListenerAttributes(0, &vecListenerPos, &vecVelocity, &vecForward, &vecUp);
	}
}
AudioPlayer&
AudioPlayer::GetInstance()
{
	if (s_pAudio == 0)
	{
		s_pAudio = new AudioPlayer();
	}

	return (*s_pAudio);
}
void
AudioPlayer::DestroyInstance()
{
	delete s_pAudio;
	s_pAudio = 0;
}