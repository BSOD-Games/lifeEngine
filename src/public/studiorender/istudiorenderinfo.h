//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ISTUDIORENDERINFO_H
#define ISTUDIORENDERINFO_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IStudioRenderInfo
	{
	public:
		virtual ~IStudioRenderInfo() {}

		virtual UInt32_t				GetCountTypesTechnique() const = 0;
		virtual const char*				GetNameTechnique( UInt32_t Index ) const = 0;
		virtual const char*				GetTypeTechnique( UInt32_t Index ) const = 0;
		virtual const char**			GetTypesTechnique() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDERINFO_H
