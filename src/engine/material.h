//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/imaterial.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Material : public IMaterial
	{
	public:
		// IMaterial
		virtual void							AddTechnique( IStudioRenderTechnique* Technique );
		virtual void							RemoveTechnique( UInt32_t Index );
		virtual void							Clear();

		virtual void							SetSurfaceName( const char* Name );

		virtual const char*						GetSurfaceName() const;
		virtual UInt32_t						GetCountTechiques() const;
		virtual IStudioRenderTechnique**		GetTechiques() const;
		virtual IStudioRenderTechnique*			GetTechnique( UInt32_t Index ) const;
		virtual IStudioRenderTechnique*			GetTechnique( RENDER_TECHNIQUE Type ) const;

		// Material
		Material();
		~Material();

	private:
		std::string										surface;
		std::vector< IStudioRenderTechnique* >			technique;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_H

