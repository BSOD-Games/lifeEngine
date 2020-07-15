//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef SOUND_H
#define SOUND_H

#include "audio/isound.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class SoundBuffer;

	//---------------------------------------------------------------------//

	class Sound : public ISound
	{
	public:
		// ISound
		virtual void				Create();
		virtual void				Delete();
		virtual void				Play();
		virtual void				Pause();
		virtual void				Stop();

		virtual void				SetLoop( bool IsLoop );
		virtual void				SetRelativeToListener( bool IsRelative );
		virtual void				SetVolume( float Volume );
		virtual void				SetPitch( float Pitch );
		virtual void				SetMinDistance( float Distance );
		virtual void				SetAttenuation( float Attenuation );
		virtual void				SetBuffer( ISoundBuffer* SoundBuffer );
		virtual void				SetPosition( const Vector3D_t& Position );

		virtual bool				IsLooped() const;
		virtual bool				IsRelativeToListener() const;
		virtual float				GetVolume() const;
		virtual float				GetPitch() const;
		virtual float				GetMinDistance() const;
		virtual float				GetAttenuation() const;
		virtual SOUND_STATUS		GetStatus() const;
		virtual ISoundBuffer*		GetBuffer() const;
		virtual Vector3D_t			GetPosition() const;

		// Sound
		Sound();
		~Sound();

	private:
		UInt32_t				handle;
		SoundBuffer*			soundBuffer;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !SOUND_H