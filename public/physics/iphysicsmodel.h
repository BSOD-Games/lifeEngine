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
		virtual void			Initialize( Vector3D_t* Verteces, UInt32_t CountVerteces, UInt32_t* Indeces, UInt32_t CountIndeces, bool IsStatic = false ) = 0;
		virtual void			Clear() = 0;

		virtual void			SetMasa( float Masa ) = 0;
		virtual void			SetInertia( const le::Vector3D_t& Inertia ) = 0;

		virtual bool			IsStatic() const = 0;
		virtual bool			IsInitialized() const = 0;
		virtual float			GetMasa() const = 0;
		virtual le::Vector3D_t	GetInertia() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define PHYSICSMODEL_INTERFACE_VERSION			"LE_PhysicsModel002"

//---------------------------------------------------------------------//

#endif // IPHYSICSMODEL_H
