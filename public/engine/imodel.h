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
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IMesh;

	//---------------------------------------------------------------------//

    class IModel : public ITransformable, public IReferenceObject
	{
	public:
        virtual ~IModel() {}
		virtual void					SetMesh( IMesh* Mesh ) = 0;
		virtual void					SetMin( const Vector3D_t& MinPosition ) = 0;
		virtual void					SetMax( const Vector3D_t& MaxPosition ) = 0;
		virtual void					SetStartFace( UInt32_t StartFace ) = 0;
		virtual void					SetCountFace( UInt32_t CountFace ) = 0;

		virtual IMesh*					GetMesh() const = 0;
		virtual const Vector3D_t&		GetMin() = 0;
		virtual const Vector3D_t&		GetMax() = 0;
		virtual UInt32_t				GetStartFace() const = 0;
		virtual UInt32_t				GetCountFace() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MODEL_INTERFACE_VERSION "LE_Model003"

//---------------------------------------------------------------------//

#endif // !IMODEL_H
