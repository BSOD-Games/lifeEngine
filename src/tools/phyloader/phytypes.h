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

struct PHYVector2D
{
	PHYVector2D( float X = 0.f, float Y = 0.f ) :
		x( X ),
		y( Y )
	{}

	PHYVector2D( const PHYVector2D& Copy ) :
		x( Copy.x ),
		y( Copy.y )
	{}

	float		x;
	float		y;
};

//---------------------------------------------------------------------//

struct PHYVector3D
{
	PHYVector3D( float X = 0.f, float Y = 0.f, float Z = 0.f ) :
		x( X ),
		y( Y ),
		z( Z )
	{}

	PHYVector3D( const PHYVector3D& Copy ) :
		x( Copy.x ),
		y( Copy.y ),
		z( Copy.z )
	{}

	float		x;
	float		y;
	float		z;
};

//---------------------------------------------------------------------//

struct PHYVector4D
{
	PHYVector4D( float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f ) :
		x( X ),
		y( Y ),
		z( Z ),
		w( W )
	{}

	PHYVector4D( const PHYVector4D& Copy ) :
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

enum PHY_LUMPS
{
	PL_PARAMETERS,
	PL_VERTECES,
	PL_INDECES,
	PL_MAX_LUMPS
};

//---------------------------------------------------------------------//

struct PHYHeader
{
	char				strId[ 4 ]; // Always 'LPHY'
	std::uint32_t		version;
};

//---------------------------------------------------------------------//

struct PHYLump
{
	std::uint32_t		offset;
	std::uint32_t		length;
};

//---------------------------------------------------------------------//

struct PHYParameters
{
	bool			isStatic;
	float			masa;
	PHYVector3D		inertia;
};

//---------------------------------------------------------------------//