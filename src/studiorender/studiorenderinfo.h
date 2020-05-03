//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef STUDIORENDERINFO_H
#define STUDIORENDERINFO_H

#include <vector>

#include "studiorender/istudiorenderinfo.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class StudioRenderInfo : public IStudioRenderInfo
	{
	public:
		// IStudioRenderInfo
		virtual UInt32_t				GetCountTypesTechnique() const;
		virtual const char*				GetNameTechnique( UInt32_t Index ) const;
		virtual const char*				GetTypeTechnique( UInt32_t Index ) const;
		virtual const char**			GetTypesTechnique() const;

		// StudioRenderInfo
		StudioRenderInfo();
		~StudioRenderInfo();
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STUDIORENDERINFO_H