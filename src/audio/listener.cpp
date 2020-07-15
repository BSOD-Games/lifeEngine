//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "global.h"
#include "listener.h"
#include "audiosystem.h"
#include "audiodevice.h"

// ------------------------------------------------------------------------------------ //
// Set global volume
// ------------------------------------------------------------------------------------ //
void le::Listener::SetGlobalVolume( float Volume )
{
	g_audioSystem->GetAudioDevice().SetGlobalVolume( Volume );
}

// ------------------------------------------------------------------------------------ //
// Set position
// ------------------------------------------------------------------------------------ //
void le::Listener::SetPosition( const Vector3D_t& Position )
{
	g_audioSystem->GetAudioDevice().SetListenerPosition( Position );
}

// ------------------------------------------------------------------------------------ //
// Set direction
// ------------------------------------------------------------------------------------ //
void le::Listener::SetDirection( const Vector3D_t& Direction )
{
	g_audioSystem->GetAudioDevice().SetListenerDirection( Direction );
}

// ------------------------------------------------------------------------------------ //
// Set up
// ------------------------------------------------------------------------------------ //
void le::Listener::SetUp( const Vector3D_t& Up )
{
	g_audioSystem->GetAudioDevice().SetListenerUp( Up );
}

// ------------------------------------------------------------------------------------ //
// Get global volume
// ------------------------------------------------------------------------------------ //
float le::Listener::GetGlobalVolume() const
{
	return g_audioSystem->GetAudioDevice().GetGlobalVolume();
}

// ------------------------------------------------------------------------------------ //
// Get position
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::Listener::GetPosition() const
{
	return g_audioSystem->GetAudioDevice().GetListenerPosition();
}

// ------------------------------------------------------------------------------------ //
// Get direction
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::Listener::GetDirection() const
{
	return g_audioSystem->GetAudioDevice().GetListenerDirection();
}

// ------------------------------------------------------------------------------------ //
// Get up
// ------------------------------------------------------------------------------------ //
le::Vector3D_t le::Listener::GetUp() const
{
	return g_audioSystem->GetAudioDevice().GetListenerUp();
}