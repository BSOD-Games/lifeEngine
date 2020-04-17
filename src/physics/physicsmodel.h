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
		virtual void				InitializeMesh(Vector3D_t* Verteces, UInt32_t CountVerteces, UInt32_t* Indeces, UInt32_t CountIndeces);
		virtual void				ClearMesh();

		virtual bool				IsInitializedMesh() const;

		// PhysicsModel
		PhysicsModel();
		~PhysicsModel();

		inline btConvexTriangleMeshShape*		GetMesh() const		{ return convexTriangleMesh; }

	private:
		UInt32_t						countReferences;
		btTriangleMesh*					triangleMesh;
		btConvexTriangleMeshShape*		convexTriangleMesh;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // PHYSICSMODEL_H
