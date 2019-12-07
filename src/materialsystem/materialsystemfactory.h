//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_SYSTEM_FACTORY_H
#define MATERIAL_SYSTEM_FACTORY_H

#include "engine/ifactory.h"

namespace le
{
	//---------------------------------------------------------------------//

	class MaterialSystemFactory : public IFactory
	{
	public:
		// IFactory
		virtual void*			Create( const char* NameInterface );
		virtual void			Delete( const char* NameInterface, void* Object );
	};

	//---------------------------------------------------------------------//
}

#endif // !MATERIAL_SYSTEM_FACTORY_H

