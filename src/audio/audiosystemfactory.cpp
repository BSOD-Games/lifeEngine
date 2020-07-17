//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "audiosystemfactory.h"
#include "soundbuffer.h"
#include "sound.h"
#include "streamsound.h"

// ------------------------------------------------------------------------------------ //
// Create object
// ------------------------------------------------------------------------------------ //
void* le::AudioSystemFactory::Create( const char* NameInterface )
{
	if ( strcmp( NameInterface, SOUNDBUFFER_INTERFACE_VERSION ) == 0 )			return new SoundBuffer();
	else if ( strcmp( NameInterface, SOUND_INTERFACE_VERSION ) == 0 )			return new Sound();
	else if ( strcmp( NameInterface, STREAMSOUND_INTERFACE_VERSION ) == 0 )		return new StreamSound();

	return nullptr;
}

// ------------------------------------------------------------------------------------ //
// Delete object
// ------------------------------------------------------------------------------------ //
void le::AudioSystemFactory::Delete( void* Object )
{
	if ( Object ) delete Object;
}
