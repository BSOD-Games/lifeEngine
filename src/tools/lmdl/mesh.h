//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

#include "common/types.h"

//---------------------------------------------------------------------//

struct aiNode;
struct aiMesh;
struct aiScene;

//---------------------------------------------------------------------//

class Mesh
{
public:
	// Mesh
	Mesh();
	~Mesh();

	void			Load( const std::string& Path );
	void			Save( const std::string& Path );
	void			Clear();

private:

	//---------------------------------------------------------------------//

	enum MDL_LUMPS
	{
		ML_MATERIALS,
		ML_VERTECES,
		ML_INDECES,
		ML_SURFACES,
		ML_MAX_LUMPS
	};

	struct MDLHeader
	{
		char				strId[ 4 ]; // Always 'LMDL'
		le::UInt32_t		version;
	};

	struct MDLLump
	{
		le::UInt32_t		offset;
		le::UInt32_t		length;
	};

	struct MDLVertex
	{
		MDLVertex();

		float		position[ 3 ];
		float		normal[ 3 ];
		float		texCoords[ 2 ];
		float		tangent[ 3 ];
		float		bitangent[ 3 ];

		bool operator==( const MDLVertex& Right );
	};

	struct MDLSurface
	{
		MDLSurface();

		le::UInt32_t		materialId;
		le::UInt32_t        startVertexIndex;
		le::UInt32_t        countVertexIndex;
	};

	//---------------------------------------------------------------------//

	void			ProcessNode( aiNode* Node, const aiScene* Scene, std::unordered_map<le::UInt32_t, std::vector< aiMesh* > >& Meshes );
	void			UpdateLumps( std::ofstream& File, MDLLump* Lumps, le::UInt32_t OffsetToLumps );
	void			WriteMaterials( std::ofstream& File, MDLLump& Lump );
	void			WriteVerteces( std::ofstream& File, MDLLump& Lump );
	void			WriteIndeces( std::ofstream& File, MDLLump& Lump );
	void			WriteSurfaces( std::ofstream& File, MDLLump& Lump );

	bool							isLoaded;
	std::vector< MDLVertex >		verteces;
	std::vector< le::UInt32_t >		indices;
	std::vector< MDLSurface >		surfaces;
	std::vector< std::string >		materials;
};

//---------------------------------------------------------------------//

#endif // !MESH_H
