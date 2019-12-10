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

#include "materialsystem/imaterial.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class MaterialVar;
	class IShader;

	//---------------------------------------------------------------------//

	class Material : public IMaterial
	{
	public:
		// IMaterial
		virtual IMaterialVar*		FindVar( const char* Name );
		virtual void				Clear();

		virtual void				SetShader( const char* Name );
		virtual void				SetSurfaceName( const char* Name );

		virtual const char*			GetShaderName() const;
		virtual const char*			GetSurfaceName() const;
		virtual UInt32_t			GetCountVars() const;
		virtual IMaterialVar*		GetVar( UInt32_t Index ) const;
		virtual IMaterialVar**		GetVars() const;

		// Material
		Material();
		~Material();

	private:
		IShader*							shader;
		std::string							surface;
		std::vector< MaterialVar* >			vars;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_H

