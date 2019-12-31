//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2019
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef IMESH_H
#define IMESH_H

#include "common/types.h"

//---------------------------------------------------------------------//

namespace le
{
	//---------------------------------------------------------------------//

	class IMaterial;
	class ITexture;
	struct MeshDescriptor;
	struct MeshSurface;

	//---------------------------------------------------------------------//

	enum PRIMITIVE_TYPE
	{
		PT_LINES,
		PT_TRIANGLES
	};

	//---------------------------------------------------------------------//

	class IMesh 
	{
	public:
		virtual void					Create( const MeshDescriptor& MeshDescriptor ) = 0;
		virtual void					Delete() = 0;

		virtual bool					IsCreated() const = 0;
		virtual UInt32_t				GetCountSurfaces() const = 0;
		virtual const MeshSurface&		GetSurface( UInt32_t Index ) const = 0;
		virtual MeshSurface*			GetSurfaces() const = 0;
		virtual UInt32_t				GetCountMaterials() const = 0;
		virtual IMaterial*				GetMaterial( UInt32_t Index ) const = 0;
		virtual IMaterial**				GetMaterials() const = 0;
		virtual UInt32_t				GetCountLightmaps() const = 0;
		virtual ITexture*				GetLightmap( UInt32_t Index ) const = 0;
		virtual ITexture**				GetLightmaps() const = 0;
	};

	//---------------------------------------------------------------------//
}

//---------------------------------------------------------------------//

#define MESH_INTERFACE_VERSION "LE_Mesh001"

//---------------------------------------------------------------------//

#endif // !IMESH_H

