//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IPHYSICSMODEL_H
#define IPHYSICSMODEL_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IPhysicsModel : public IReferenceObject
	{
	public:
		virtual ~IPhysicsModel() {}
		virtual void			InitializeMesh( Vector3D_t* Verteces, UInt32_t CountVerteces, UInt32_t* Indeces, UInt32_t CountIndeces ) = 0;
		virtual void			ClearMesh() = 0;

		virtual bool			IsInitializedMesh() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define PHYSICSMODEL_INTERFACE_VERSION			"LE_PhysicsModel001"

//---------------------------------------------------------------------//

#endif // IPHYSICSMODEL_H
