//
//  File Name   :   AudioPlayer.h
//  Description :   Class of CParticleSystem
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __AUDIOPLAYER_H__
#define __AUDIOPLAYER_H__

// Library Includes
#include "../Bin/Tools/FMOD Api/inc/fmod.hpp"

// Local Includes
#include "mathlibrary.h"

// Types
enum E3DSoundType
{
	SOUND_INVALID = -1,
	SOUND_BIRDCHIRP,
	SOUND_FOOTSTEPS,
	SOUND_BUTTONPRESS,
	SOUND_MAX
};

// Constants

// Prototypes

class AudioPlayer	//			SINGLETON
{
	//Member functions
public:
	virtual ~AudioPlayer();

	virtual bool Initialise( bool _bLoadSounds = true );
	virtual void LoadSounds();
	virtual void Process();

	virtual void Play3DSound( E3DSoundType _eSound, Math::Vector3& _vecPosition );
	virtual void SetListenerPosition( Math::Vector3& _rVecListenerPos, Math::Vector3& _rVecForward, Math::Vector3& _rVecUp );

	//Singleton Methods
	static AudioPlayer& GetInstance();
	static void DestroyInstance();

private:
	AudioPlayer();
	AudioPlayer( const AudioPlayer& _krInstanceToCopy );
	const AudioPlayer& operator =( const AudioPlayer& _krInstanceToCopy );

	//Member variables
protected:
	static AudioPlayer* s_pAudio;

	FMOD::System* m_pFMod;
	FMOD::Channel* m_pChannel;
	FMOD::Sound** m_pSound;

	bool m_bSoundsAreLoaded;
};


#endif //__CLOTH_H__