//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIAL_H
#define IMATERIAL_H

#include "common/types.h"
#include "studiorender/istudiorendertechnique.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IStudioRenderTechnique;

	//---------------------------------------------------------------------//

	class IMaterial
	{
	public:
		virtual ~IMaterial() {}
		virtual void							AddTechnique( IStudioRenderTechnique* Technique ) = 0;
		virtual void							RemoveTechnique( UInt32_t Index ) = 0;
		virtual void							Clear() = 0;

		virtual void							SetSurfaceName( const char* Name ) = 0;

		virtual const char*						GetSurfaceName() const = 0;
		virtual UInt32_t						GetCountTechiques() const = 0;
		virtual IStudioRenderTechnique**		GetTechiques() const = 0;
		virtual IStudioRenderTechnique*			GetTechnique( UInt32_t Index ) const = 0;
		virtual IStudioRenderTechnique*			GetTechnique( RENDER_TECHNIQUE Type ) const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MATERIAL_INTERFACE_VERSION "LE_Material003"

//---------------------------------------------------------------------//

#endif // !IMATERIAL_H
