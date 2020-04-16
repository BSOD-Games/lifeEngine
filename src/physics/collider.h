//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>
#include <btBulletCollisionCommon.h>

#include "physics/icollider.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Collider : public ICollider
	{
	public:
		// IReferenceObject
		virtual void			IncrementReference();
		virtual void			DecrementReference();
		virtual void			Release();

		virtual UInt32_t		GetCountReferences() const;

		// ICollider
		virtual void					AddShape( const ShapeBoxDescriptor& Shape, const Matrix4x4_t& LocalTransormation );
		virtual void					AddShape( const ShapeCylinderDescriptor& Shape, const Matrix4x4_t& LocalTransormation );
		virtual void					AddShape( const ShapeCapsuleDescriptor& Shape, const Matrix4x4_t& LocalTransormation );
		virtual void					AddShape( const ShapeSphereDescriptor& Shape, const Matrix4x4_t& LocalTransormation );
		virtual void					AddShape( const ShapeMeshDescriptor& Shape, const Matrix4x4_t& LocalTransormation );
		virtual void					AddShape( const ShapeConeDescriptor& Shape, const Matrix4x4_t& LocalTransormation );
		virtual void					AddShape( const ShapeConvexHullDescriptor& Shape, const Matrix4x4_t& LocalTransormation );
		virtual void					RemoveShape( UInt32_t Index );
		virtual void					RemoveAllShapes();
		virtual void					Scale( const Vector3D_t& FactorScale );

		virtual void					SetScale( const Vector3D_t& Scale );

		virtual void					GetAABB( Vector3D_t& Min, Vector3D_t& Max ) const;
		virtual UInt32_t				GetCountShapes() const;
		virtual const Vector3D_t&		GetScale() const;

		// Collider
		Collider();
		~Collider();

		inline btCollisionShape&		GetCollisionShape() const		{ return ( btCollisionShape& ) shape; }

	private:

		//---------------------------------------------------------------------//

		enum SHAPE_TYPE
		{
			ST_BOX,
			ST_CYLINDER,
			ST_CAPSULE,
			ST_SPHERE,
			ST_MESH,
			ST_CONE,
			ST_CONVEX_HULL
		};

		//---------------------------------------------------------------------//

		struct ColliderMeshDescriptor
		{
			ColliderMeshDescriptor();
			~ColliderMeshDescriptor();

			btTriangleMesh*				triangleMesh;
			btBvhTriangleMeshShape*		shape;
		};

		//---------------------------------------------------------------------//

		struct ShapeDescriptor
		{
			SHAPE_TYPE		type;
			void*			shape;
		};

		//---------------------------------------------------------------------//

		UInt32_t								countReferences;
		Vector3D_t								scale;;
		std::vector< ShapeDescriptor >			shapesDescriptors;
		btCompoundShape							shape;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !COLLIDER_H
