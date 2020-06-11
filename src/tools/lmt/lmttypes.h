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

//---------------------------------------------------------------------//

struct LMTVector2D
{
	LMTVector2D( float X = 0.f, float Y = 0.f ) :
		x( X ),
		y( Y )
	{}

	LMTVector2D( const LMTVector2D& Copy ) :
		x( Copy.x ),
		y( Copy.y )
	{}

	float		x;
	float		y;
};

//---------------------------------------------------------------------//

struct LMTVector3D
{
	LMTVector3D( float X = 0.f, float Y = 0.f, float Z = 0.f ) :
		x( X ),
		y( Y ),
		z( Z )
	{}

	LMTVector3D( const LMTVector3D& Copy ) :
		x( Copy.x ),
		y( Copy.y ),
		z( Copy.z )
	{}

	float		x;
	float		y;
	float		z;
};

//---------------------------------------------------------------------//

struct LMTVector4D
{
	LMTVector4D( float X = 0.f, float Y = 0.f, float Z = 0.f, float W = 0.f ) :
		x( X ),
		y( Y ),
		z( Z ),
		w( W )
	{}

	LMTVector4D( const LMTVector4D& Copy ) :
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

struct LMTColor
{
	LMTColor( float R = 0.f, float G = 0.f, float B = 0.f, float A = 0.f ) :
		r( R ),
		g( G ),
		b( B ),
		a( A )
	{}

	LMTColor( const LMTColor& Copy ) :
		r( Copy.r ),
		g( Copy.g ),
		b( Copy.b ),
		a( Copy.a )
	{}

	float		r;
	float		g;
	float		b;
	float		a;
};

//---------------------------------------------------------------------//