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
        // IReferenceObject
        virtual void                            IncrementReference();
        virtual void                            DecrementReference();
        virtual void                            Release();
        virtual UInt32_t                        GetCountReferences() const;

		// IMesh
		virtual void							Create( const MeshDescriptor& MeshDescriptor );
        virtual void                            Update( const void* Verteces, UInt32_t SizeVerteces, UInt32_t OffsetVerteces );
        virtual void                            Update( UInt32_t* Indeces, UInt32_t CountIndeces, UInt32_t StartIdIndex );
        virtual void                            Update( MeshSurface* Surfaces, UInt32_t CountSurfaces, UInt32_t StartIdSurface );
        virtual void                            Update( IMaterial** Materials, UInt32_t CountMaterials, UInt32_t StartIdMaterial );
        virtual void                            Update( ITexture** Lightmaps, UInt32_t CountLighmaps, UInt32_t StartIdLightmap );
		virtual void							Delete();

		virtual bool							IsCreated() const;
		virtual UInt32_t						GetCountSurfaces() const;
		virtual const MeshSurface&				GetSurface( UInt32_t Index ) const;
		virtual MeshSurface*					GetSurfaces() const;
		virtual UInt32_t						GetCountMaterials() const;
		virtual IMaterial*						GetMaterial( UInt32_t Index ) const;
		virtual IMaterial**						GetMaterials() const;
		virtual UInt32_t						GetCountLightmaps() const;
		virtual ITexture*						GetLightmap( UInt32_t Index ) const;
		virtual ITexture**						GetLightmaps() const;
		virtual const Vector3D_t&				GetMin() const;
		virtual const Vector3D_t&				GetMax() const;

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
		Vector3D_t						min;
		Vector3D_t						max;
        UInt32_t                        countReferences;

		std::vector< IMaterial* >		materials;
		std::vector< ITexture* >		lightmaps;
		std::vector< MeshSurface >		surfaces;
    };

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#endif // !IMESH_H

