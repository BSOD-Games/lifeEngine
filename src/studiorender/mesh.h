//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MESH_H
#define MESH_H

#include <vector>

#include "studiorender/imesh.h"
#include "vertexarrayobject.h"
#include "vertexbufferobject.h"
#include "indexbufferobject.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class Mesh : public IMesh
	{
	public:
		// IMesh
		virtual void							Create( const MeshDescriptor& MeshDescriptor );
		virtual void							Delete();

		virtual bool							IsCreated() const;
		virtual UInt32_t						GetCountSurfaces() const;
		virtual const MeshSurface&				GetSurface( UInt32_t Index ) const;
		virtual MeshSurface*					GetSurfaces() const;
		virtual UInt32_t						GetCountMaterials() const;
		virtual IMaterial*						GetMaterial( UInt32_t Index ) const;
		virtual IMaterial**						GetMaterials() const;

		// Mesh
		Mesh();
		~Mesh();

		inline PRIMITIVE_TYPE					GetPrimitiveType() const		{ return primitiveType; }
		inline const VertexArrayObject&			GetVertexArrayObject() const	{ return vertexArrayObject; }
		inline const VertexBufferObject&		GetVertexBufferObject() const	{ return vertexBufferObject; }
		inline const IndexBufferObject&			GetIndexBufferObject() const	{ return indexBufferObject; }

	private:
		bool							isCreated;
		PRIMITIVE_TYPE					primitiveType;

		VertexArrayObject				vertexArrayObject;
		VertexBufferObject				vertexBufferObject;
		IndexBufferObject				indexBufferObject;

		std::vector< IMaterial* >		materials;
		std::vector< MeshSurface >		surfaces;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMESH_H

