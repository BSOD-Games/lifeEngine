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

		//---------------------------------------------------------------------//

		struct TechniqueInfo
		{
			const char*			type;
			const char*			name;
		};

		//---------------------------------------------------------------------//

		struct PassInfo
		{
			const char*			type;
			const char*			name;
		};

		//---------------------------------------------------------------------//

		virtual ~IStudioRenderInfo() {}

		virtual UInt32_t				GetCountTechniques() const = 0;
		virtual UInt32_t				GetCountPasses( UInt32_t IDTechnique ) const = 0;
		virtual TechniqueInfo			GetTechnique( UInt32_t IDTechnique ) const = 0;
		virtual PassInfo				GetPass( UInt32_t IDTechnique, UInt32_t IDPass ) const = 0;
		virtual TechniqueInfo*			GetTechniques() const = 0;
		virtual PassInfo*				GetPasses( UInt32_t IDTechnique ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !ISTUDIORENDERINFO_H
