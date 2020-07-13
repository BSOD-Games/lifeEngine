//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IAUDIOSYSTEM_H
#define IAUDIOSYSTEM_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IFactory;

	//---------------------------------------------------------------------//

	class IAudioSystem
	{
	public:
		virtual ~IAudioSystem() {}
		//virtual void				SetGlobalVolume( float Volume ) = 0;
		//virtual void				SetListenerPosition( const Vector3D_t& Position ) = 0;
		//virtual void				SetListenerDirection( const Vector3D_t& Direction ) = 0;
		//virtual void				SetListenerUpVector( const Vector3D_t& Up ) = 0;

		//virtual float				GetGlobalVolume() const = 0;
		//virtual Vector3D_t			GetListenerPosition() const = 0;
		//virtual Vector3D_t			GetListenerDirection() const = 0;
		//virtual Vector3D_t			GetListenerUpVector() const = 0;
		virtual IFactory*			GetFactory() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IAUDIOSYSTEM_H