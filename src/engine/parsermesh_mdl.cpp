//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-GameEdition/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#include "common/meshsurface.h"
#include "common/meshdescriptor.h"
#include "engine/iresourcesystem.h"
#include "studiorender/imesh.h"
#include "studiorender/studiovertexelement.h"

#include "global.h"
#include "mdldoc.h"
#include "parsermesh_mdl.h"
#include "consolesystem.h"

// ------------------------------------------------------------------------------------ //
// Release
// ------------------------------------------------------------------------------------ //
void le::ParserMeshMDL::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------ //
// Get version
// ------------------------------------------------------------------------------------ //
const char* le::ParserMeshMDL::GetVersion() const
{
	return "3.0";
}

// ------------------------------------------------------------------------------------ //
// Get name
// ------------------------------------------------------------------------------------ //
const char* le::ParserMeshMDL::GetName() const
{
	return "lifeEngine models";
}

// ------------------------------------------------------------------------------------ //
// Get count file extensions
// ------------------------------------------------------------------------------------ //
le::UInt32_t le::ParserMeshMDL::GetCountFileExtensions() const
{
	return 1;
}

// ------------------------------------------------------------------------------------ //
// Get file extensions
// ------------------------------------------------------------------------------------ //
const char** le::ParserMeshMDL::GetFileExtensions() const
{
	static const char*			extensions[ 1 ] = { "mdl" };
	return extensions;
}

// ------------------------------------------------------------------------------------ //
// Get author
// ------------------------------------------------------------------------------------ //
const char* le::ParserMeshMDL::GetAuthor() const
{
	return "Egor Pogulyaka";
}

// ------------------------------------------------------------------------------------ //
// Read
// ------------------------------------------------------------------------------------ //
le::IMesh* le::ParserMeshMDL::Read( const char* Path, IResourceSystem* ResourceSystem, IFactory* StudioRenderFactory )
{
	MDLDoc				mdlDoc;
	if ( !mdlDoc.Load( Path ) )
	{
		g_consoleSystem->PrintError( "Failed loading model" );
		return nullptr;
	}

	// Getting all data
	auto&				verteces = mdlDoc.GetVerteces();
	auto&				vertexIndeces = mdlDoc.GetVertexIndeces();
	auto&				materialPaths = mdlDoc.GetMaterials();
	auto&				surfaces = mdlDoc.GetSurfaces();
	MDLVector3D			minXYZ = mdlDoc.GetMinXYZ();
	MDLVector3D			maxXYZ = mdlDoc.GetMaxXYZ();

	// Loading materials
	std::vector< le::IMaterial* >		materials;
	for ( UInt32_t index = 0, count = materialPaths.size(); index < count; ++index )
	{
		IMaterial*			material = ResourceSystem->LoadMaterial( materialPaths[ index ].c_str(), materialPaths[ index ].c_str() );
		if ( !material ) continue;

		materials.push_back( material );
	}

	// Convert MDLSurface to le::MeshSurface
	std::vector< MeshSurface >			meshSurfaces;
	for ( UInt32_t index = 0, count = surfaces.size(); index < count; ++index )
	{
		const MDLSurface&		mdlSurface = surfaces[ index ];
		MeshSurface				surface;

		surface.startVertexIndex = 0;
		surface.lightmapID = 0;
		surface.materialID = mdlSurface.materialId;
		surface.startIndex = mdlSurface.startVertexIndex;
		surface.countIndeces = mdlSurface.countVertexIndeces;
		meshSurfaces.push_back( surface );
	}

	IMesh* mesh = ( le::IMesh* ) StudioRenderFactory->Create( MESH_INTERFACE_VERSION );
	if ( !mesh )
	{
		g_consoleSystem->PrintError( "Interface le::IMesh [%s] not founded in studiorender", MESH_INTERFACE_VERSION );
		return nullptr;
	}

	// Create descriptor format verteces
	std::vector< StudioVertexElement >			vertexElements =
	{
		{ 3, VET_FLOAT },		// Position
		{ 3, VET_FLOAT },		// Normal
		{ 2, VET_FLOAT },		// TexCoord
		{ 3, VET_FLOAT },		// Tangent
		{ 3, VET_FLOAT }		// Bitangent
	};

	// Creating mesh descriptor and loading to gpu
	le::MeshDescriptor				meshDescriptor;
	meshDescriptor.countIndeces = vertexIndeces.size();
	meshDescriptor.countMaterials = materials.size();
	meshDescriptor.countLightmaps = 0;
	meshDescriptor.countSurfaces = meshSurfaces.size();
	meshDescriptor.sizeVerteces = verteces.size() * sizeof( MDLVertex );

	meshDescriptor.indeces = ( le::UInt32_t* ) vertexIndeces.data();
	meshDescriptor.materials = materials.data();
	meshDescriptor.lightmaps = nullptr;
	meshDescriptor.surfaces = meshSurfaces.data();
	meshDescriptor.verteces = verteces.data();

	meshDescriptor.min = Vector3D_t( minXYZ.x, minXYZ.y, minXYZ.z );
	meshDescriptor.max = Vector3D_t( maxXYZ.x, maxXYZ.y, maxXYZ.z );
	meshDescriptor.primitiveType = PT_TRIANGLES;
	meshDescriptor.countVertexElements = vertexElements.size();
	meshDescriptor.vertexElements = vertexElements.data();

	// Loading mesh to gpu
	mesh->Create( meshDescriptor );
	if ( !mesh->IsCreated() )
	{
		mesh->Release();
		
		g_consoleSystem->PrintError( "Mesh not created" );
		return nullptr;
	}

	return mesh;
}
