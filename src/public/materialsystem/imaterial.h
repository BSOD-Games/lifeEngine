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

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IMaterialVar;

	//---------------------------------------------------------------------//

	class IMaterial
	{
	public:
		virtual void			AddVar( IMaterialVar* MaterialVar ) = 0;
		virtual IMaterialVar*	FindVar( const char* Name ) = 0;
		virtual void			Clear() = 0;

		virtual void			SetName( const char* Name ) = 0;
		virtual void			SetShader( const char* Name ) = 0;

		virtual const char*		GetName() const = 0;
		virtual const char*		GetShaderName() const = 0;
		virtual UInt32_t		GetCountVars() const = 0;
		virtual IMaterialVar*	GetVar( UInt32_t Index ) const = 0;
		virtual IMaterialVar**	GetVars() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MATERIAL_INTERFACE_VERSION "LE_Material001"

//---------------------------------------------------------------------//

#endif // !IMATERIAL_H

