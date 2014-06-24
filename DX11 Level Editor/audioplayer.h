//
//  File Name   :   CAudioPlayer.h
//  Description :   Class of CParticleSystem
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __AUDIOPLAYER_H__
#define __AUDIOPLAYER_H__

// Library Includes
#include "../Bin/Tools/FMOD Api/inc/fmod.hpp"
#include <D3DX10.h>

// Local Includes

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

class CAudioPlayer
{
//Member functions
public:
	virtual ~CAudioPlayer();

	virtual bool Initialise(bool _bLoadSounds = true);
	virtual void LoadSounds();
	virtual void Process();

	virtual void Play3DSound(E3DSoundType _eSound, D3DXVECTOR3& _vecPosition);
	virtual void SetListenerPosition(D3DXVECTOR3& _rVecListenerPos, D3DXVECTOR3& _rVecForward, D3DXVECTOR3& _rVecUp);

	//Singleton Methods
	static CAudioPlayer& GetInstance();
	static void DestroyInstance();
	
private:
	CAudioPlayer();
	CAudioPlayer(const CAudioPlayer& _krInstanceToCopy);
	const CAudioPlayer& operator =(const CAudioPlayer& _krInstanceToCopy);

//Member variables
protected:
	static CAudioPlayer* s_pAudio;

	FMOD::System* m_pFMod;
	FMOD::Channel* m_pChannel;
	FMOD::Sound** m_pSound;

	bool m_bSoundsAreLoaded;
};


#endif //__CLOTH_H__