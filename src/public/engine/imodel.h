//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMODEL_H
#define IMODEL_H

#include "engine/itransformable.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IMesh;

	//---------------------------------------------------------------------//

	class IModel : public ITransformable
	{
	public:
		virtual void					SetMesh( IMesh* Mesh ) = 0;
		virtual void					SetMin( const Vector3D_t& MinPosition ) = 0;
		virtual void					SetMax( const Vector3D_t& MaxPosition ) = 0;

		virtual IMesh*					GetMesh() const = 0;
		virtual const Vector3D_t&		GetMin() const = 0;
		virtual const Vector3D_t&		GetMax() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MODEL_INTERFACE_VERSION "LE_Model001"

//---------------------------------------------------------------------//

#endif // !IMODEL_H