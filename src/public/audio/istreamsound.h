//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTREAMSOUND_H
#define ISTREAMSOUND_H

#include "common/types.h"
#include "engine/ireferenceobject.h"
#include "audio/isound.h"
#include "audio/soundstatus.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IParserSoundBuffer;

	//---------------------------------------------------------------------//

	class IStreamSound : public IReferenceObject
	{
	public:
		virtual ~IStreamSound() {}
		virtual void			Create() = 0;
		virtual void			Delete() = 0;
		virtual void			Play() = 0;
		virtual void			Pause() = 0;
		virtual void			Stop() = 0;
		virtual void			Open( IParserSoundBuffer* ParserSoundBuffer ) = 0;
		virtual void			Close() = 0;

		virtual void			SetLoop( bool IsLoop ) = 0;
		virtual void			SetRelativeToListener( bool IsRelative ) = 0;
		virtual void			SetVolume( float Volume ) = 0;
		virtual void			SetPitch( float Pitch ) = 0;
		virtual void			SetMinDistance( float Distance ) = 0;
		virtual void			SetAttenuation( float Attenuation ) = 0;	
		virtual void			SetPosition( const Vector3D_t& Position ) = 0;

		virtual bool			IsOpened() const = 0;
		virtual bool			IsLooped() const = 0;
		virtual bool			IsRelativeToListener() const = 0;
		virtual float			GetVolume() const = 0;
		virtual float			GetPitch() const = 0;
		virtual float			GetMinDistance() const = 0;
		virtual float			GetAttenuation() const = 0;
		virtual SOUND_STATUS	GetStatus() const = 0;
		virtual Vector3D_t		GetPosition() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define STREAMSOUND_INTERFACE_VERSION "LE_StreamSound001"

//---------------------------------------------------------------------//

#endif // !ISTREAMSOUND_H