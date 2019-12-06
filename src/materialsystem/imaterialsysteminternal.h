//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIALSYSTEM_INTERNAL_H
#define IMATERIALSYSTEM_INTERNAL_H

#include "materialsystem/imaterialsystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IMaterialSystemInternal : public IMaterialSystem
	{
	public:
		virtual bool			Initialize( IEngine* Engine ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIALSYSTEM_INTERNAL_H

