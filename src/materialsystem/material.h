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
		virtual void				AddVar( IMaterialVar* MaterialVar );
		virtual IMaterialVar*		FindVar( const char* Name );
		virtual void				Clear();

		virtual void				SetName( const char* Name );
		virtual void				SetShader( const char* Name );

		virtual const char*			GetName() const;
		virtual const char*			GetShaderName() const;
		virtual UInt32_t			GetCountVars() const;
		virtual IMaterialVar*		GetVar( UInt32_t Index ) const;
		virtual IMaterialVar**		GetVars() const;

		// Material
		Material();

	private:
		std::string							name;
		IShader*							shader;
		std::vector< MaterialVar* >			vars;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !MATERIAL_H

