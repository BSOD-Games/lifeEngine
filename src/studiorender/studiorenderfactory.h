//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDER_FACTORY_H
#define STUDIORENDER_FACTORY_H

#include "engine/ifactory.h"

namespace le
{
	//---------------------------------------------------------------------//

	class StudioRenderFactory : public IFactory
	{
	public:
		// IFactory
		virtual void*			Create( const char* NameInterface );
		virtual void			Delete( const char* NameInterface, void* Object );
	};

	//---------------------------------------------------------------------//
}

#endif // !STUDIORENDER_FACTORY_H

