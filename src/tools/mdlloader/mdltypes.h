//////////////////////////////////////////////////////////////////////////
//
//			*** lifeEngine (Двигатель жизни) ***
//				Copyright (C) 2018-2020
//
// Репозиторий движка:  https://github.com/BSOD-Games/lifeEngine
// Авторы:				Егор Погуляка (zombiHello)
//
//////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

//---------------------------------------------------------------------//

enum MDL_LUMPS
{
	ML_MATERIALS,
	ML_VERTECES,
	ML_INDECES,
	ML_SURFACES,
	ML_MAX_LUMPS
};

//---------------------------------------------------------------------//

struct MDLHeader
{
	char				strId[ 4 ]; // Always 'LMDL'
	std::uint32_t		version;
};

//---------------------------------------------------------------------//

struct MDLLump
{
	std::uint32_t		offset;
	std::uint32_t		length;
};

//---------------------------------------------------------------------//

struct MDLVector2D
{
	MDLVector2D( float X = 0.f, float Y = 0.f ) :
		x( X ),
		y( Y )
	{}

	MDLVector2D( const MDLVector2D& Copy ) :
		x( Copy.x ),
		y( Copy.y )
	{}

	float		x;
	float		y;
};

//---------------------------------------------------------------------//

struct MDLVector3D
{
	MDLVector3D( float X = 0.f, float Y = 0.f, float Z = 0.f ) :
		x( X ),
		y( Y ),
		z( Z )
	{}

	MDLVector3D( const MDLVector3D& Copy ) :
		x( Copy.x ),
		y( Copy.y ),
		z( Copy.z )
	{}

	float		x;
	float		y;
	float		z;
};

//---------------------------------------------------------------------//

struct MDLVector4D
{
	MDLVector4D( float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f ) :
		x( X ),
		y( Y ),
		z( Z ),
		w( W )
	{}

	MDLVector4D( const MDLVector4D& Copy ) :
		x( Copy.x ),
		y( Copy.y ),
		z( Copy.z ),
		w( Copy.w )
	{}

	float		x;
	float		y;
	float		z;
	float		w;
};

//---------------------------------------------------------------------//

struct MDLVertex
{
	MDLVertex()
	{}

	MDLVertex( const MDLVertex& Copy ) :
		position( Copy.position ),
		normal( Copy.normal ),
		texCoords( Copy.texCoords ),
		tangent( Copy.tangent ),
		bitangent( Copy.bitangent )
	{}

	MDLVector3D		position;
	MDLVector3D		normal;
	MDLVector2D		texCoords;
	MDLVector3D		tangent;
	MDLVector3D		bitangent;
};

//---------------------------------------------------------------------//

struct MDLSurface
{
	MDLSurface( std::uint32_t MaterialID = 0, std::uint32_t StartVertexIndex = 0, std::uint32_t CountVertexIndeces = 0) :
		materialId( MaterialID ),
		startVertexIndex( StartVertexIndex ),
		countVertexIndeces( CountVertexIndeces )
	{}

	MDLSurface( const MDLSurface& Copy ) :
		materialId( Copy.materialId ),
		startVertexIndex( Copy.startVertexIndex ),
		countVertexIndeces( Copy.countVertexIndeces )
	{}

	std::uint32_t			materialId;
	std::uint32_t			startVertexIndex;
	std::uint32_t			countVertexIndeces;
};

//---------------------------------------------------------------------//