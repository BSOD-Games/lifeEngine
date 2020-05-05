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
		virtual UInt32_t				GetCountTechniques() const;
		virtual UInt32_t				GetCountPasses( UInt32_t IDTechnique ) const;
		virtual TechniqueInfo			GetTechnique( UInt32_t IDTechnique ) const;
		virtual PassInfo				GetPass( UInt32_t IDTechnique, UInt32_t IDPass ) const;
		virtual TechniqueInfo*			GetTechniques() const;
		virtual PassInfo*				GetPasses( UInt32_t IDTechnique ) const;

		// StudioRenderInfo
		StudioRenderInfo();
		~StudioRenderInfo();

	private:
		std::vector< le::StudioRenderInfo::TechniqueInfo >					techniques;
		std::vector< std::vector< le::StudioRenderInfo::PassInfo > >		passes;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !STUDIORENDERINFO_H