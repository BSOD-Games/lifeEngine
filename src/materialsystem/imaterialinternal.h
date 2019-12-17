//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMATERIAL_INTERNAL_H
#define IMATERIAL_INTERNAL_H

#include "materialsystem/imaterial.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class ICamera;

	//---------------------------------------------------------------------//

	class IMaterialInternal : public IMaterial
	{
	public:
		virtual void			OnDrawMesh( const Matrix4x4_t& Transformation, ICamera* Camera ) = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMATERIAL_INTERNAL_H

