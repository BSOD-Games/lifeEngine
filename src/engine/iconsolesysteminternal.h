//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICONSOLESYSTEM_INTERNAL_H
#define ICONSOLESYSTEM_INTERNAL_H

#include "engine/iconsolesystem.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngine;

	//---------------------------------------------------------------------//

	class IConsoleSystemInternal : public IConsoleSystem
	{
	public:
		virtual void		Initialize() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ICONSOLESYSTEM_INTERNAL_H

