//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef PHYSICSMODEL_H
#define PHYSICSMODEL_H

#include <vector>

#include "physics/iphysicsmodel.h"

//---------------------------------------------------------------------//

class btTriangleMesh;
class btConvexTriangleMeshShape;

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class PhysicsModel : public IPhysicsModel
	{
	public:
		// IReferenceObject
		virtual void				IncrementReference();
		virtual void				DecrementReference();
		virtual void				Release();
		virtual UInt32_t			GetCountReferences() const;

		// IPhysicsModel
		virtual void				Initialize( Vector3D_t* Verteces, UInt32_t CountVerteces, UInt32_t* Indeces, UInt32_t CountIndeces, bool IsStatic = false );
		virtual void				Clear();

		virtual void				SetMasa( float Masa );
		virtual void				SetInertia( const le::Vector3D_t& Inertia );

		virtual bool				IsStatic() const;
		virtual bool				IsInitialized() const;
		virtual float				GetMasa() const;
		virtual le::Vector3D_t		GetInertia() const;

		// PhysicsModel
		PhysicsModel();
		~PhysicsModel();

		inline btCollisionShape*		GetShape() const		{ return shape; }

	private:
		bool							isStatic;
		float							masa;
		UInt32_t						countReferences;
		Vector3D_t						inertia;

		btTriangleIndexVertexArray*		mesh;
		btCollisionShape*				shape;
		std::vector< Vector3D_t >		verteces;
		std::vector< UInt32_t >			indeces;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // PHYSICSMODEL_H
