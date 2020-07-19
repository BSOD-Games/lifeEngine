//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2020
//
// ����������� ������:  https://github.com/BSOD-GameEdition/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef AUDIOSYSTEMFACTORY_H
#define AUDIOSYSTEMFACTORY_H

#include "engine/ifactory.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class AudioSystemFactory : public IFactory
	{
	public:
		//IFactory
		virtual void*			Create( const char* NameInterface );
		virtual void			Delete( void* Object );
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !AUDIOSYSTEMFACTORY_H