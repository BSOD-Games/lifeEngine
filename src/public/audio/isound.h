//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISOUND_H
#define ISOUND_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	enum SOUND_STATUS
	{
		SS_PLAYING,
		SS_PAUSED,
		SS_STOPED
	};

	//---------------------------------------------------------------------//

	class ISoundBuffer;

	//---------------------------------------------------------------------//

	class ISound
	{
	public:
		virtual ~ISound() {}
		virtual void					Create() = 0;
		virtual void					Delete() = 0;
		virtual void					Play() = 0;
		virtual void					Pause() = 0;
		virtual void					Stop() = 0;

		//virtual void					SetLoop( bool IsLoop ) = 0;
		//virtual void					SetVolume( float Volume ) = 0;
		virtual void					SetBuffer( ISoundBuffer* SoundBuffer ) = 0;
		//virtual void					SetPosition( const Vector3D_t& Position ) = 0;

		//virtual bool					IsLooped() const = 0;
		//virtual float					GetVolume() const = 0;
		virtual SOUND_STATUS			GetStatus() const = 0;
		virtual ISoundBuffer*			GetBuffer() const = 0;
		//virtual Vector3D_t				GetPosition() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define SOUND_INTERFACE_VERSION "LE_Sound001"

//---------------------------------------------------------------------//

#endif // !ISOUND_H
