//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef ICOLLIDER_H
#define ICOLLIDER_H

#include "common/types.h"
#include "engine/ireferenceobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	struct ShapeBoxDescriptor;
	struct ShapeCylinderDescriptor;
	struct ShapeCapsuleDescriptor;
	struct ShapeSphereDescriptor;
	struct ShapeMeshDescriptor;
	struct ShapeConeDescriptor;
	struct ShapeConvexHullDescriptor;

	//---------------------------------------------------------------------//

	class ICollider : public IReferenceObject
	{
	public:
		virtual ~ICollider() {}
		virtual void			AddShape( const ShapeBoxDescriptor& Shape, const Matrix4x4_t& LocalTransormation ) = 0;
		virtual void			AddShape( const ShapeCylinderDescriptor& Shape, const Matrix4x4_t& LocalTransormation ) = 0;
		virtual void			AddShape( const ShapeCapsuleDescriptor& Shape, const Matrix4x4_t& LocalTransormation ) = 0;
		virtual void			AddShape( const ShapeSphereDescriptor& Shape, const Matrix4x4_t& LocalTransormation ) = 0;
		virtual void			AddShape( const ShapeMeshDescriptor& Shape, const Matrix4x4_t& LocalTransormation ) = 0;
		virtual void			AddShape( const ShapeConeDescriptor& Shape, const Matrix4x4_t& LocalTransormation ) = 0;
		virtual void			AddShape( const ShapeConvexHullDescriptor& Shape, const Matrix4x4_t& LocalTransormation ) = 0;
		virtual void			RemoveShape( UInt32_t Index ) = 0;
		virtual void			RemoveAllShapes() = 0;

		virtual void			GetAABB( Vector3D_t& Min, Vector3D_t& Max ) const = 0;
		virtual UInt32_t		GetCountShapes() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define COLLIDER_INTERFACE_VERSION "LE_Collider002"

//---------------------------------------------------------------------//

#endif // !ICOLLISION_H
