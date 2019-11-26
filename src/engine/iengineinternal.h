//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (��������� �����) ***
//				Copyright (C) 2018-2019
//
// ����������� ������:  https://github.com/zombihello/lifeEngine
// ������:				���� �������� (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IENGINE_INTERNAL_H
#define IENGINE_INTERNAL_H

#include "engine/lifeengine.h"
#include "engine/iengine.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IEngineInternal : public IEngine
	{
	public:
		virtual bool		Initialize( WindowHandle_t WindowHandle = nullptr ) = 0;
		virtual bool		LoadGame( const char* DirGame ) = 0;
		virtual void		UnloadGame() = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IENGINE_INTERNAL_H